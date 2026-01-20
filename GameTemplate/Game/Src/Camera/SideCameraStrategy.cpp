#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Camera/SideCameraStrategy.h"

namespace
{
    // 共通設定
    const float FOLLOW_START_POSITION = 9.0f; // 追従ライン
    const float FOOT_TO_CENTER_OFFSET = 1.0f;
    const float FOLLOW_SPEED = 5.0f; // 追従速度

    // 通常ステージ用設定 (既存の値を維持)
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
    {
        offsetViewPos.y = FOLLOW_START_POSITION;
    }
    g_camera3D->SetPosition(offsetViewPos);

    //// 注視点の計算 //////////////////////////////////////////////
    Vector3 lookAtTarget = targetPos;
    lookAtTarget.y = offsetViewPos.y; // 水平を見る
    g_camera3D->SetTarget(lookAtTarget);
}


void SideCameraStrategy::BossCamera()
{
    // --- ボス戦: 完全固定カメラ ---

    float currentHeight = g_camera3D->GetHeight();

    float nextHeight = LerpFloat(currentHeight, BOSS_FIXED_HEIGHT, FOLLOW_SPEED);
    InitCameraAspect(nextHeight);

    Vector3 idealPos = BOSS_FIXED_TARGET + BOSS_OFFSET;

    // 座標の適用
    Vector3 currentCamPos = g_camera3D->GetPosition();
    Vector3 nextPos = Lerp(FOLLOW_SPEED * g_gameTime->GetFrameDeltaTime(), currentCamPos, idealPos);
    g_camera3D->SetPosition(nextPos);

    // 注視点も固定
    Vector3 lookAt = nextPos;
    lookAt.z = BOSS_FIXED_TARGET.z;
    g_camera3D->SetTarget(lookAt);
}
