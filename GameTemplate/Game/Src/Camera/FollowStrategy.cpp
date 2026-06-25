#include "stdafx.h"

#include "FollowStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/BossUIManager.h"

namespace
{
    // 追従ライン（これ以下ならカメラ位置の基準は下がらない）
    // 通常ステージ。
    const auto THRESHOLD_Y_STAGE = 100.0f;

    // ボス戦ステージ。。
    const auto THRESHOLD_Y_BOSS = 15.0f;

    const float CAMERA_DISTANCE = 20.0f;       // 25〜35 で調整（奥行きの見え方）
    const float FIXED_PITCH_DEG = 22.0f;      // 原作寄り俯角 22〜28
    const float STICK_YAW_SENSITIVITY = 0.0f; // 0 = 完全固定（原作どおり）

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

    const float CLIMB_TRANSITION_TIME = 1.0f;
} // namespace


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
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = g_camera3D->GetPosition();

    if (pPlayer_->GetCharacterController().IsOnGround())
        lastGroundY_ = targetPos.y;
    if (lastGroundY_ == 0.0f && targetPos.y > 0.0f)
        lastGroundY_ = targetPos.y;

    // ① 真後ろ（SPM / BossCamera と同じ）
    Vector3 idealOffset(0.0f, 0.0f, -CAMERA_DISTANCE);

    // ② 俯角だけかける（stickY は使わない）
    Quaternion qPitch;
    qPitch.SetRotationDeg(Vector3::AxisX, FIXED_PITCH_DEG);
    qPitch.Apply(idealOffset);

    // ③ 次元切替の 90°（CameraManager の INIT_CAMERA_ANGLE_3D）
    targetRotation_.Apply(idealOffset);

    // ④ プレイヤー位置 + オフセット
    const Vector3 idealPos = targetPos + idealOffset;

    const float destY = CalculateThresholdY(targetPos.y, THRESHOLD_Y_STAGE, idealOffset.y);

    const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
    Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPos);
    newPos.y = LerpFloat(currentCamPos.y, destY, 5.0f);

    ApplyScreenRock(newPos);
    g_camera3D->SetPosition(newPos);

    // ⑤ 注視点：プレイヤー足元付近（原作はキャラ＋先の道）
    Vector3 lookAtPoint = targetPos;
    lookAtPoint.y += 1.5f;
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

    const Vector3 currentCamPos = g_camera3D->GetPosition();
    const auto deltaTime = g_gameTime->GetFrameDeltaTime();

    // 通常の追従速度
    float followSpeed = 15.0f;

    // 単純なLerpで追従
    Vector3 nextPos = Lerp(followSpeed * deltaTime, currentCamPos, idealPos_);

    ApplyScreenRock(nextPos);
    g_camera3D->SetPosition(nextPos);
    g_camera3D->SetTarget(lookAtPoint_);
}

