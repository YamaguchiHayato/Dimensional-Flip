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
    const auto THRESHOLD_Y_BOSS = 15.0f;

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

    bool isBossBattleRange = false;
    if (pBoss_)
    {
        // プレイヤーの座標チェック（ボス固定位置 X=0 から離れすぎていないか）
        if (pPlayer_->GetPlayerPos().x < 100.0f)
            isBossBattleRange = true;
    }

    if (pBoss_ && isBossBattleRange)
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
    targetPos_ = pPlayer_->GetPlayerPos();

    // スティック入力
    float stickX = g_pad[0]->GetRStickXF();
    float stickY = g_pad[0]->GetRStickYF() * -1.0f;

    // 回転速度
    float rotSpeed = 100.0f * g_gameTime->GetFrameDeltaTime();

    // 角度を加算（蓄積）
    orbitAngleY_ += stickX * rotSpeed;
    orbitAngleX_ += stickY * rotSpeed;

    // 上下の角度制限（-80度～80度くらいが一般的ですが、地面潜り防止で調整）
    if (orbitAngleX_ > 60.0f)
        orbitAngleX_ = 60.0f;
    if (orbitAngleX_ < -20.0f)
        orbitAngleX_ = -20.0f;

    float distance = 25.0f;

    // ① 基準となるベクトル（真後ろ）
    Vector3 orbitOffset = Vector3(0.0f, 0.0f, -distance);

    Quaternion qPitch;
    qPitch.SetRotationDeg(Vector3::AxisX, 20.0f + orbitAngleX_);
    qPitch.Apply(orbitOffset);

    // ③ Y軸回転（左右・Yaw）
    Quaternion qYaw;
    qYaw.SetRotationDeg(Vector3::AxisY, orbitAngleY_);
    qYaw.Apply(orbitOffset);

    // ④ 次元反転などの外部回転があれば適用
    targetRotation_.Apply(orbitOffset);


    // 注視点（プレイヤーの少し上）
    lookAtPoint_ = targetPos_;
    lookAtPoint_.y += 7.5f;

    // 理想座標 = 注視点 + 計算したオフセット
    idealPos_ = lookAtPoint_ + orbitOffset;

    // ---------------------------------------------------------
    // 4. ボスダウン（疲労）時の特殊カメラ上書き
    // ---------------------------------------------------------
    isBossTumbler_ = (pBoss_->GetCurrentState() == pBoss_->GetStateList()[app::enemyStatus::state_Tumble]);
    if (isBossTumbler_ && targetPos_.y > THRESHOLD_Y_BOSS)
    {
        MakeClimbingPerspective();
    }

    const Vector3 currentCamPos = g_camera3D->GetPosition();
    const auto deltaTime = g_gameTime->GetFrameDeltaTime();

    // ボスダウン時は追従を速く、通常時は少し滑らかに
    auto followSpeed = isBossTumbler_ ? 10.0f : 15.0f;

    Vector3 nextPos = Lerp(followSpeed * deltaTime, currentCamPos, idealPos_);

    ApplyScreenRock(nextPos);
    g_camera3D->SetPosition(nextPos);
    g_camera3D->SetTarget(lookAtPoint_);
}

void FollowStrategy::MakeClimbingPerspective()
{
    // Playerの移動ベクトルを取得。
    auto moveVec = pPlayer_->GetMoveSpeed();

    if (moveVec.Length() > 0.001f || targetPos_.y > THRESHOLD_Y_BOSS)
    {
        // プレイヤーの回転を取得。
        // 足場を登る際のみ、Playerの真後ろあたりのベクトルを設定。
        currentPlayerRot_ = pPlayer_->GetPlayerRotation();
    }

    backOffset_ = Vector3::Front;
    currentPlayerRot_.Apply(backOffset_);

    // 最終的なオフセットを計算。
    auto climbDist = OFFSET_CLIMB.z - 10.0f;
    finalOffset_ = backOffset_ * climbDist;

    // XZ軸の同期。
    idealPos_.x = targetPos_.x + finalOffset_.x;
    idealPos_.z = targetPos_.z + finalOffset_.z;

    // Y軸の同期。
    auto followY = targetPos_.y + OFFSET_CLIMB.y;
    idealPos_.y = (followY > BOSS_FIXED_POSS.y) ? followY : BOSS_FIXED_POSS.y + 10.0f;

    // 注視点の追従。
    frontOffset_ = Vector3::Front;
    currentPlayerRot_.Apply(frontOffset_);
    lookAtPoint_ = targetPos_ + (frontOffset_ * 20.0f);
    lookAtPoint_.y += 2.0f;
}



