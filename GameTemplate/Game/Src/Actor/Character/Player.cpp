#include "stdafx.h"

// TODO: 将来的にはコマンドパターンとして設計。

// キャラクタークラス。
#include "Src/Actor/Character/Player.h"


// カメラクラス。
#include "Src/Core/CameraManager.h"
#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"


// UIクラス。
#include "Src/UI/ScoreUI.h"


// 内部ステータスクラス。
#include "Src/Actor/Character/Status.h"

// プレイヤーステータス構造体。
struct PlauerStatus
{
    static const uint8_t MAX_HP = 8;           // 最大体力。
    static constexpr auto SPEED = 150.0f;      // 移動速度。
    static constexpr auto JUMP_POWER = 350.0f; // ジャンプ力。
    static constexpr auto GLAVITY = 15.0f;     // 重力。
    static constexpr auto ATTACK_POWER = 1.0f; // 攻撃力。
};

namespace
{
    const Vector3 SCALE(0.5f, 0.5f, 0.5f);

    // モデルのファイルパス。
    const char* PLAYER_MODEL = "Assets/modelData/";
    const char* PLAYER_ANIMATION = "Assets/animData/"; // ファイルパス。
    const char* ANIMATION_FILE_EXTENSION = ".tka";     // 拡張子。
    const char* MODEL_FILE_EXTENSION = ".tkm";         // 拡張子。
}


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
    render_.Init("Assets/modelData/unityChan.tkm", animationClip_, EnAnimationClip::animNum, enModelUpAxisY);
    charaCon_.Init(20.0f, 25.0f, pos_);

    // プレイヤーのステータスを初期化する。
    status_.Initial(PlauerStatus::MAX_HP, PlauerStatus::SPEED, PlauerStatus::ATTACK_POWER);
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

        if (!currentStrategy)
            return;

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
    moveSpeed_.x = 0.0f;
    moveSpeed_.z = 0.0f;

    // 今のカメラモードを取得する。
    CameraMode currentMode = pCameraManager_->GetCurrentCameraMode();
    // 基準を決める。
    // 移動速度。
    auto currentSpeed = PlauerStatus::SPEED;
    // ジャンプ力。
    auto currentJumpPower = PlauerStatus::JUMP_POWER;

    if (currentMode == CameraMode::modeStageEX || currentMode == CameraMode::modeBoss)
    {
        currentSpeed = 450.0f;     // 速度 3倍
        currentJumpPower = 600.0f; // ジャンプ力 強化 (通常350 -> 600くらい)
    }

    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    // カメラモード判定 (BossMode系)
    if (currentMode == CameraMode::modeBoss || currentMode == CameraMode::mode3D || currentMode == CameraMode::modeStageEX)
    {
        Vector3 camForward = g_camera3D->GetForward();
        Vector3 camRight = g_camera3D->GetRight();
        camForward.y = 0.0f;
        camRight.y = 0.0f;
        camForward.Normalize();
        camRight.Normalize();
        Vector3 targetMove = (camForward * stickL.y) + (camRight * stickL.x);
        moveSpeed_ += targetMove * currentSpeed;
    }


    else
    {
        Vector3 right = g_camera3D->GetRight();
        right.y = 0.0f;

        moveSpeed_ += right * (stickL.x * currentSpeed);
    }

    if (charaCon_.IsOnGround())
    {
        moveSpeed_.y = 0.0f;

        if (g_pad[0]->IsTrigger(enButtonA))
        {
            moveSpeed_.y = currentJumpPower;
            didJumpThisFrame_ = true;
        }
    }
    // 重力の設定。
    moveSpeed_.y -= PlauerStatus::GLAVITY;
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
        {
            // 待機アニメーションの再生
            render_.PlayAnimation(EnAnimationClip::animIdle);
            break;
        }
         
        // 歩き状態だったら
        case PlayerState::sJump:
        {
            // ジャンプアニメーションの再生
            render_.PlayAnimation(EnAnimationClip::animJump);
            break;
        }
         
        // ジャンプ中だったら
        case PlayerState::sRun:
        {
            render_.PlayAnimation(EnAnimationClip::animRun);
            break;
        }
    }
}


void Player::Render(RenderContext& rc)
{
    render_.Draw(rc);
}


void Player::SetAnimation()
{
    // 待機アニメーション。
    FetchPlayAnimation(EnAnimationClip::animIdle, "idle", true);
    // 走りアニメーション。
    FetchPlayAnimation(EnAnimationClip::animRun, "run", true);
    // ジャンプアニメーション。
    FetchPlayAnimation(EnAnimationClip::animJump, "jump", false);
}
