#include "stdafx.h"
#include "FollowStrategy.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"

#include "Src/Core/BossUIManager.h"



namespace
{
    // 追従ライン（これ以下ならカメラ位置の基準は下がらない）
    // 通常ステージ。
    const auto THRESHOLD_Y_STAGE = 100.0f;

    // ボス戦ステージ。。
    const auto THRESHOLD_Y_BOSS = 20.0f;

    // 回転速度。
    const auto ROT_SPEED = 2.0f;

    // カメラのオフセット値。
    // 通常ステージ。
    const Vector3 OFFSET_BASE(0.0f, 15.0f, -15.0f);

    // ボス戦ステージ。
    const Vector3 OFFSET_CLIMB(0.0f, 30.0f, -30.0f);

    // 俯瞰視点の基準位置。
    const Vector3 BOSS_FIXED_POSS = {0.0f, 40.0f, -50.0f};
    const Vector3 BOSS_FIXED_LOOKAT = {0.0f, 5.0f, 0.0f};
}


FollowStrategy::FollowStrategy(Player* pPlayer)
{
    pPlayer_ = pPlayer;
    SetTargetRotationY(0.0f);
}


bool FollowStrategy::Start()
{
	////ニアクリップとファークリップの設定
	g_camera3D->SetNear(10.0f);         
	// スカイキューブの大きさに合わせて調整
    // 700 
	g_camera3D->SetFar(10000.0f);      


    pBoss_ = FindGO<app::enemy::Boss>("boss");
	return true;
}


void FollowStrategy::Update()
{
    if (!pPlayer_)
        return;

    if (!pBoss_)
        // ボスが居ないなら探す。
        pBoss_ = FindGO<app::enemy::Boss>("boss");

    if (pBoss_)
        BossCamera();

    else
        StageCamera();
}


void FollowStrategy::ApplyScreenRock(Vector3& cameraPos)
{
    if (!isScreenRock_)
        return;

    // X軸の制限
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


void FollowStrategy::StageCamera()
{
    // 取得。
    // 座標。
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = g_camera3D->GetPosition();
    // スティック
    const float stickX = g_pad[0]->GetRStickXF();
    const float stickY = g_pad[0]->GetRStickYF() * -1.0f;

    if (pPlayer_->GetCharacterController().IsOnGround())
        lastGroundY_ = targetPos.y;
    // ズレ防止用に初期化。
    if (lastGroundY_ == 0.0f && targetPos.y > 0.0f)
        lastGroundY_ = targetPos.y;

    Vector3 idealOffset = OFFSET_BASE;

    // Y軸回転
    Quaternion rotY;
    rotY.SetRotationDeg(Vector3::AxisY, 1.3f * stickX);
    rotY.Apply(idealOffset);
    targetRotation_.Apply(idealOffset); // 外部回転があれば適用

    // X軸回転
    Vector3 axisX = Cross(Vector3::AxisY, idealOffset);
    axisX.Normalize();
    Quaternion rotX;
    rotX.SetRotationDeg(axisX, 1.3f * stickY);
    rotX.Apply(idealOffset);

    // 高さ制限
    float desiredHeightOffset = idealOffset.y;
    Vector3 idealPosXZ = targetPos + idealOffset;

    // プレイヤーの座標を計算。
    float destY = CalculateThresholdY(targetPos.y, THRESHOLD_Y_STAGE, desiredHeightOffset);

    // カメラ位置を補完。
    const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
    Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPosXZ);

    // Y座標の補完処理。
    float nextY = LerpFloat(currentCamPos.y, destY, 5.0f);

    // Y座標を適用
    newPos.y = nextY;

    // スクリーンロックの適用。
    ApplyScreenRock(newPos);

    g_camera3D->SetPosition(newPos);

    // 注視点 (プレイヤーの少し上を見る)
    Vector3 lookAtPoint = targetPos;
    // + する値は要調整。
    lookAtPoint.y = lastGroundY_ + 7.5f;
    g_camera3D->SetTarget(lookAtPoint);
}


void FollowStrategy::BossCamera()
{
    // 座標を取得する。
    // Player。
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    // カメラ。
    const Vector3 currentCamPos = g_camera3D->GetPosition();
    // デルタタイム。
    const auto deltaTime = g_gameTime->GetFrameDeltaTime();

    // 目標位置。
    Vector3 idealPos = BOSS_FIXED_POSS;
    Vector3 lookAtPoint = BOSS_FIXED_LOOKAT;

    // ボスが疲労ステートに入ったかどうか確認。
    isBossTumbler_ = (pBoss_->GetCurrentState() == pBoss_->GetStateList()[app::enemyStatus::state_Tumble]);
    if (isBossTumbler_ && targetPos.y > THRESHOLD_Y_BOSS)
    {
        // プレイヤーの回転を取得。
        currentPlayerRot_ = pPlayer_->GetPlayerRotation();

        backOffset_ = Vector3::Front * OFFSET_CLIMB.z;
        currentPlayerRot_.Apply(backOffset_);

        // XZ軸の同期。
        idealPos.x = targetPos.x + OFFSET_CLIMB.x;
        idealPos.z = targetPos.z + OFFSET_CLIMB.z;

        // Y座標のみ高さを補正する。
        idealPos.y = BOSS_FIXED_POSS.y + OFFSET_CLIMB.y;

        frontOffset_ = Vector3::Front * 10.0f; // nf先を見る。
        currentPlayerRot_.Apply(frontOffset_);


        // 注視点の追従。
        lookAtPoint = targetPos;
        lookAtPoint.y += 5.0f;
    }

    // 注視点もPlayerに合わせて動かす。
    // @ TODO: 要調整。
    auto followSpeed = isBossTumbler_ ? 10.0f : 5.0f;
    Vector3 nextPos = Lerp(followSpeed * deltaTime , currentCamPos, idealPos);

    // スクリーンロックの適応。
    ApplyScreenRock(nextPos);
    g_camera3D->SetPosition(nextPos);


    // 注視店の設定。
    g_camera3D->SetTarget(lookAtPoint);
}



