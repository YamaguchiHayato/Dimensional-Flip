#include "stdafx.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"
#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/UI/ScoreUI.h"

namespace
{
    // モデルのスケール。
    const Vector3 SCALE(0.5f, 0.5f, 0.5f);
    // モデルにかかる重力。
    const float GLAVITY = 15.0f;
    // モデルのファイルパス。
    const char* PLAYER_MODEL = "Assets/modelData/";
    const char* PLAYER_ANIMATION = "Assets/animData/"; // ファイルパス。
    const char* ANIMATION_FILE_EXTENSION = ".tka";     // 拡張子。
    const char* MODEL_FILE_EXTENSION = ".tkm";         // 拡張子。
} // namespace

const std::string Player::FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName,bool flag)
{
    std::string AnimationFilePath = PLAYER_ANIMATION + animationName + ANIMATION_FILE_EXTENSION;

    animationClip_[enAnimationClip].Load(AnimationFilePath.c_str());
    animationClip_[enAnimationClip].SetLoopFlag(flag);
    return AnimationFilePath;
}

const std::string Player::FetchPlayerModel(const std::string& modelName, AnimationClip animationClip,EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag)
{
    // モデルをロード(tkmファイル名を打ち込む)。
    std::string Player = PLAYER_MODEL + modelName + MODEL_FILE_EXTENSION;

    animationClip_[enAnimationClip].Load(Player.c_str());
    animationClip_[enAnimationClip].SetLoopFlag(flag);
    return Player;
}

bool Player::Start()
{
    // アニメーションの設定。
    SetAnimation();
    render_.Init("Assets/modelData/unityChan.tkm", animationClip_, EnAnimationClip::Num, enModelUpAxisY);
    charaCon_.Init(20.0f, 25.0f, pos_);
    return true;
}

void Player::Update()
{
    if (isPaused_)
        return;
    didJumpThisFrame_ = false;

    Move();
    Action();
    Rotation();
    PlayAnimation();
    ManageState();
    render_.SetScale(SCALE);
    render_.SetPosition(pos_);
    render_.Update();
}

void Player::Action()
{
    // カメラマネージャーのポインタを取得。
    if (!pCameraManager_)
        return;

    // 現在のカメラモードを取得。
    CameraMode currentMode = pCameraManager_->GetCurrentCameraMode();
    // トリガーエリア内かどうか取得。
    bool isInTriggerArea = GetInTriggerArea();

    // ボタンアクション。
    // Bボタンで2Dカメラ回転トグル (エリア内 かつ 2Dモード)
    if (g_pad[0]->IsTrigger(enButtonB) && isInTriggerArea && currentMode == CameraMode::mode2_5D)
    {
        // 今のカメラモードを取得。
        ICameraStrategy* currentStrategy = pCameraManager_->GetCurrentStrategy();
        SideCameraStrategy* sideStrategy = dynamic_cast<SideCameraStrategy*>(currentStrategy);
        if (sideStrategy)
        {
            float currentTargetAngle = sideStrategy->GetTargetRotationY();

            // トグル操作。
            if (fabsf(currentTargetAngle - 90.0f) < 1.0f)
                pCameraManager_->Request3DModeRot(0.0f);
            else
                pCameraManager_->Request3DModeRot(90.0f);
        }
    }

    // Xボタンで元の2Dモード要求 (エリア内 かつ 3Dモード)
    else if (g_pad[0]->IsTrigger(enButtonX) && isInTriggerArea && currentMode == CameraMode::mode3D)
        pCameraManager_->Request2DMode();
}

void Player::Move()
{
    // 1. 水平移動速度をリセット
    moveSpeed_.x = 0.0f;
    moveSpeed_.z = 0.0f;

    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    ChangeDimensionCamera();

    if (charaCon_.IsOnGround())
    {
        moveSpeed_.y = 0.0f;

        if (g_pad[0]->IsTrigger(enButtonA))
        {
            moveSpeed_.y = 350.0f;
            didJumpThisFrame_ = true;
        }
    }
    // 重力の設定。
    moveSpeed_.y -= GLAVITY;
    // 移動速度。
    pos_ = charaCon_.Execute(moveSpeed_, 1.0f / 150.0f);
    // 座標のセット。
    charaCon_.SetPosition(pos_);
    render_.SetPosition(pos_);
}

void Player::Move3Dmode()
{
    // スティックの取得。
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    //// 移動方式。
    //// 上下移動。
    moveSpeed_.x += stickL.y * 480.0f;
    //// 左右移動。
    moveSpeed_.z += stickL.x * 480.0f;
    //反転して正常の向きに修正
    moveSpeed_.z *= -1;
}

void Player::Move2_5Dmode()
{
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    Vector3 right = g_camera3D->GetRight();
    right.y = 0.0f;

    moveSpeed_ += right * (stickL.x * 480.0f);
}

void Player::ChangeDimensionCamera()
{
    // カメラのモードを設定、初期値は2D
    CameraMode currentMode = CameraMode::mode2_5D;

    if (pCameraManager_)
        // 現在のカメラのモードを取得
        currentMode = pCameraManager_->GetCurrentCameraMode();

    // 3Dモードの処理
    if (currentMode == CameraMode::mode2_5D && g_pad[0]->IsTrigger(enButtonB))
    {
        is3DMode_ = true;
    }

    if (is3DMode_)
    {
        Move3Dmode();
    }
    else
    {
        Move2_5Dmode();
        is3DMode_ = false;
    }
}

void Player::Rotation()
{
    Vector3 dir = moveSpeed_;

    if (fabsf(dir.x) >= 0.001f || fabsf(dir.z) >= 0.001f)
    {
        rot_.SetRotationYFromDirectionXZ(dir);
        render_.SetRotation(rot_);
    }
}

// ステート管理
void Player::ManageState()
{
    // 地面についていなかったら
    if (charaCon_.IsOnGround() == false)
    {
        // ステートを1にする
        state_ = PlayerState::sJump;
        return;
    }

    // 地面に着地したら
    // x zの移動速度があったらスティックの入力
    if (fabsf(moveSpeed_.x) >= 0.001f || fabsf(moveSpeed_.z) >= 0.001f)
        state_ = PlayerState::sRun;

    // 何も入力しなかったら
    else
        state_ = PlayerState::sIdle;
}

void Player::PlayAnimation()
{
    // switch文
    switch (state_)
    {
    // 待機状態だったら
    case PlayerState::sIdle:
        // 待機アニメーションの再生
        render_.PlayAnimation(EnAnimationClip::Idle);
        break;

    // 歩き状態だったら
    case PlayerState::sJump:
        render_.PlayAnimation(EnAnimationClip::Jump);
        break;

    // ジャンプ中だったら
    case PlayerState::sRun:
        render_.PlayAnimation(EnAnimationClip::Run);
        break;
    }
}

void Player::Render(RenderContext& rc)
{
    render_.Draw(rc);
}

void Player::SetAnimation()
{
    // 待機アニメーション。
    FetchPlayAnimation(EnAnimationClip::Idle, "idle", true);
    // 走りアニメーション。
    FetchPlayAnimation(EnAnimationClip::Run, "run", true);
    // ジャンプアニメーション。
    FetchPlayAnimation(EnAnimationClip::Jump, "jump", false);
}
