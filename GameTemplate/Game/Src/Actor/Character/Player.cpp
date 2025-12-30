#include "stdafx.h"

#include "DimensionManager.h"

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
#include "Src/Core/SoundManager.h"

#include "Src/Actor/Character/Player/Character2DRender.h"

#include "Src/Actor/Character/Player/PlayerIdleState.h"
#include "Src/Actor/Character/Player/PlayerRunState.h"
#include "Src/Actor/Character/Player/PlayerJumpState.h"
#include "Src/Actor/Character/Player/PlayerFallState.h"

// プレイヤーステータス構造体。
struct PlayerStatus
{
    static const uint8_t MAX_HP = 8;               // 最大体力。
    static constexpr auto ATTACK_POWER = 1.0f;     // 攻撃力。


    struct Move
    {
        static constexpr float SPEED = 100.0f; // 移動速度アップ
    };


    // ジャンプ用のパラメータ。
    struct Jump
    {
        static constexpr float GLAVITY = 2.5f;         // 落下中の重力倍率。
        static constexpr float CUT = 2.5f;             // ボタンを押した後の重力倍率。。
    };
};


namespace
{
    const Vector3 SCALE = Vector3(0.125f, 0.125f, 0.125f);
    const uint8_t MAX_NUM = 8; // モデルの数。

    // モデルのファイルパス。
    const char* PATH = "Assets/modelData/girl/facing_/"; // ファイルパス。
    const char* FILE_EXTENSION = ".tkm";                 // 拡張子。

     // モデルのパスリスト。
     const std::string PARH_LIST[MAX_NUM] =
     {
         // 左。
        "Assets/modelData/girl/facing_left/L_idle.tkm",
        "Assets/modelData/girl/facing_left/L_jump.tkm",
        "Assets/modelData/girl/facing_left/L_run_front_legRight.tkm",
        "Assets/modelData/girl/facing_left/L_run_front_legLeft.tkm",


        // 右。
        "Assets/modelData/girl/facing_right/R_idle.tkm",
        "Assets/modelData/girl/facing_right/R_jump.tkm",
        "Assets/modelData/girl/facing_right/R_run_front_legRight.tkm",
        "Assets/modelData/girl/facing_right/R_run_front_legLeft.tkm",
    };


    static Character2DRender* render;

    const auto CURRENT_TIME = 0.0f;
    const auto MAX_TIME = 0.8f;


    // モデルのファイルパス。
    const char* PLAYER_ANIMATION = "Assets/animData/"; // ファイルパス。
    const char* ANIMATION_FILE_EXTENSION = ".tka";     // 拡張子。
}



const std::string Player::FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName,bool flag)
{
    std::string AnimationFilePath = PLAYER_ANIMATION + animationName + ANIMATION_FILE_EXTENSION;

    animationClip_[enAnimationClip].Load(AnimationFilePath.c_str());
    animationClip_[enAnimationClip].SetLoopFlag(flag);
    return AnimationFilePath;
}


bool Player::Start()
{
    // ステートの登録。
    RegisterState<app::state::PlayerIdleState>(enState_Idle);
    RegisterState<app::state::PlayerRunState>(enState_Run);
    RegisterState<app::state::PlayerJumpState >(enState_Jump);
    RegisterState<app::state::PlayerFallState >(enState_Fall);

    // 初期ステートの設定。
    pCurrentState_ = pStateArray_[enState_Idle];
    pCurrentState_->Enter();

    // 移動速度。
    walkSpeed_ = PlayerStatus::Move::SPEED;

    // モデルの初期化。
    pRender_ = new Character2DRender();
    pRender_->Init(std::vector<std::string>(PARH_LIST, PARH_LIST + MAX_NUM));

    charaCon_.Init(2.0f, 1.0f, pos_);
    // プレイヤーのステータスを初期化する。
    status_.Initial(PlayerStatus::MAX_HP, walkSpeed_, PlayerStatus::ATTACK_POWER);


    // プレイヤーの座標を描画。
    posFont_.SetPosition({-600.0f, 300.0f, 0.0f});
    posFont_.SetScale(1.0f);                     // 文字の大きさ
    posFont_.SetColor({1.0f, 1.0f, 1.0f, 1.0f}); // 白文字


    SetRespwanPos(pos_);
    return true;
}


void Player::Update()
{
    if (isPaused_)
        return;


    if (IsDimensionSwitchAction())
        app::core::DimensionManager::GetInstance()->FlipDimension(pCameraManager_,app::collision::CollisionManager::GetInstance());
    


    ////////// 更新処理。///////////
    _ASSERT(pCurrentState_ != nullptr);

    uint8_t request;
    if (pCurrentState_->RequestID(request))
    {
        pCurrentState_->Exit();
        pCurrentState_ = pStateArray_[request];
        pCurrentState_->Enter();
    }
    pCurrentState_->Update();

    if (pRender_)
    {
        pRender_->SetCurrentIndex(currentIndex);
        pRender_->SetScale(SCALE);
        pRender_->Update();
    }
    /////////////////////////////////

    didJumpThisFrame_ = false;

    // カメラの状態に応じてOffset回転を取る    
    CameraOffsetRot();

    // リスポーンチェック。
    CheckRespawn();

    // プレイヤーの回転をセット。
    Rotation();

    // 座標表示の更新。
    wchar_t text[256];
    swprintf_s(text, L"Player Pos\nX: %.1f\nY: %.1f\nZ: %.1f", pos_.x, pos_.y, pos_.z);
    posFont_.SetText(text);
}


void Player::CameraOffsetRot()
{
    {
        const auto cameraType = pCameraManager_->GetCurrentCameraMode();

        if (cameraType == CameraMode::mode2D)
            offsetRot_.SetRotationY(0.0f);

        else
            offsetRot_.SetRotationY(-90.0f);
    }
}


bool Player::IsDimensionSwitchAction()
{
    // カメラマネージャーを取得する。
    if (!pCameraManager_)
        return false;

    bool isInTriggerArea = GetInTriggerArea();
    auto currentMode = pCameraManager_->GetCurrentCameraMode();

    // カメラトリガー内かつ 2.5Dモードのときかつ Bボタンが押されたら。
    if (isInTriggerArea && g_pad[0]->IsTrigger(enButtonB))
    {
        // 2Dまたは3Dの時のみ切り替えを許可（ボス戦中などは防ぐ）
        if (currentMode == CameraMode::mode2D || currentMode == CameraMode::mode3D)
        {
            return true;
        }
    }
    return false;
}


void Player::ApplyMovement()
{
    auto ficedTime = 1.0f / 150.0f;

    // 移動処理。
    pos_ = charaCon_.Execute(moveSpeed_, ficedTime);

    // 座標のセット。
    charaCon_.SetPosition(pos_);


    if (pRender_)
        pRender_->SetPosition(pos_);

}


void Player::Rotation()
{
    rot_ = offsetRot_;
    pRender_->SetRotation(rot_);
}


void Player::Render(RenderContext& rc)
{
    // キャラモデル。
    pRender_->Render(rc);

    // 座標表示。    
//    posFont_.Draw(rc);
}


void Player::SetAnimation()
{
    // アイドルアニメーション。
    FetchPlayAnimation(EnAnimationClip::animIdle, "idle", true);
    // 走るアニメーション。
    FetchPlayAnimation(EnAnimationClip::animRun, "run", true);
    // ジャンプアニメーション。
    FetchPlayAnimation(EnAnimationClip::animJump, "jump", false);
}


void Player::CheckRespawn()
{
    const auto line = -100.0f;

    if (pos_.y < line)
    {
        SetPlayerPos(respwanPos_);
        moveSpeed_ = Vector3::Zero;
        respawnFlag_ = true;
    }
}


