#include "stdafx.h"
#include <algorithm>

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Camera/SideCameraStrategy.h"


namespace
{
    // 共通設定
    const float FOLLOW_START_POSITION = 9.0f; // 追従ライン
    const float FOOT_TO_CENTER_OFFSET = 1.0f;
    const float FOLLOW_SPEED = 5.0f; // 追従速度

    // 通常ステージ用設定
    const Vector3 STAGE_OFFSET = {0.0f, 0.0f, -200.0f}; // 通常は近め
    const float STAGE_HEIGHT = 25.0f;                   // 通常の画角

    // ボス戦用。
    const Vector3 BOSS_OFFSET = {0.0f, 0.0f, -1000.0f}; // 奥行き
    const Vector3 BOSS_FIXED_TARGET = {0.0f, 18.0f, 0.0f};
    const float BOSS_FIXED_HEIGHT = 42.0f;

}


SideCameraStrategy::SideCameraStrategy(Player* pPlayer)
{
    // 視点のアスペクト比を設定
    InitCameraAspect(STAGE_HEIGHT);

    pPlayer_ = pPlayer;
    SetTargetRotationY(0.0f);

    // 近平面と遠平面の設定
    g_camera3D->SetNear(100.0f);
    // スカイキューブの大きさに合わせて調整
    g_camera3D->SetFar(5000.0f);
}


bool SideCameraStrategy::Start()
{
    // Bossクラスを探索。
    pSecondTarget_ = FindGO<app::enemy::Boss>("boss");

    return true;
}


void SideCameraStrategy::Update()
{
    if (!pPlayer_)
        return;

    // Bossがまだ見つかっていない場合は探索を続ける。
    if (!pSecondTarget_)
    {
        pSecondTarget_ = FindGO<app::enemy::Boss>("boss");

        // 見つかった瞬間だけ、カメラを定位置にワープさせる（初期化）
        if (pSecondTarget_)
        {
            Vector3 idealPos = BOSS_FIXED_TARGET + BOSS_OFFSET;
            g_camera3D->SetPosition(idealPos);

            Vector3 lookAt = idealPos;
            lookAt.z = BOSS_FIXED_TARGET.z;
            g_camera3D->SetTarget(lookAt);

            InitCameraAspect(BOSS_FIXED_HEIGHT);
        }
    }

    // ボスがいるか居ないかで2Dカメラの種類を切り替える。
    if (pSecondTarget_)
        BossCamera();
    else
        StageCamera();
}


void SideCameraStrategy::StageCamera()
{
    // ズームをデフォルトに戻す(念のため)
    InitCameraAspect(STAGE_HEIGHT);

    //// 視点の計算 ////////////////////////////////////////////////
    // プレイヤーの位置の取得
    const Vector3 targetPos = pPlayer_->GetPlayerPos();

    // オフセットの適用(視点を手前に引く)
    Vector3 offsetViewPos = targetPos + STAGE_OFFSET;

    offsetViewPos.y = targetPos.y + FOOT_TO_CENTER_OFFSET;
    if (targetPos.y <= FOLLOW_START_POSITION - FOOT_TO_CENTER_OFFSET)
        offsetViewPos.y = FOLLOW_START_POSITION;

    ApplyScreenRock(offsetViewPos);

    g_camera3D->SetPosition(offsetViewPos);

    //// 注視点の計算 //////////////////////////////////////////////
    Vector3 lookAtTarget = targetPos;
    lookAtTarget.y = offsetViewPos.y; // 水平を見る
    g_camera3D->SetTarget(lookAtTarget);
}


void SideCameraStrategy::BossCamera()
{
    // 画角の更新
    float nextHeight = LerpFloat(g_camera3D->GetHeight(), BOSS_FIXED_HEIGHT, FOLLOW_SPEED);
    InitCameraAspect(nextHeight);

    // カメラ座標の滑らかな更新
    Vector3 idealPos = BOSS_FIXED_TARGET + BOSS_OFFSET;
    Vector3 nextPos = Lerp(FOLLOW_SPEED * g_gameTime->GetFrameDeltaTime(), g_camera3D->GetPosition(), idealPos);

    // スクリーンロックの適用
    ApplyScreenRock(nextPos);
    g_camera3D->SetPosition(nextPos);

    // 注視点の決定と制限
    Vector3 lookAt = nextPos;
    lookAt.z = BOSS_FIXED_TARGET.z; // 奥行きは固定

    // 注視点も画面端で止める (if文で代用)
    if (isScreenRock_)
    {
        if (lookAt.x < rangeMin_.x)
            lookAt.x = rangeMin_.x;
        if (lookAt.x > rangeMax_.x)
            lookAt.x = rangeMax_.x;
    }

    g_camera3D->SetTarget(lookAt);
}


void SideCameraStrategy::ApplyScreenRock(Vector3& cameraPos)
{
    if (!isScreenRock_)
        return;

    // X軸の制限 (if文で代用)
    if (cameraPos.x < rangeMin_.x)
        cameraPos.x = rangeMin_.x;
    if (cameraPos.x > rangeMax_.x)
        cameraPos.x = rangeMax_.x;

    // Z軸の制限
    if (cameraPos.z < rangeMin_.z)
        cameraPos.z = rangeMin_.z;
    if (cameraPos.z > rangeMax_.z)
        cameraPos.z = rangeMax_.z;
}
