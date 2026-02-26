#include "stdafx.h"

#include "Src/Core/InputManager.h"
#include "Src/Core/Game.h"

// キャラクタークラス。
#include "Src/Actor/Character/Player/Player.h"


// カメラクラス。
#include "Src/Core/CameraManager.h"
#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"

// UIクラス。
#include "Src/UI/ScoreUI.h"
#include "Src/Production/CutIn/CutInView.h"

// 内部ステータスクラス。
#include "Src/Actor/Character/Status.h"
#include "Src/Core/SoundManager.h"

#include "Src/Actor/Character/Player/Character2DRender.h"

// ステートクラス。
#include "Src/Actor/Character/Player/PlayerIdleState.h"
#include "Src/Actor/Character/Player/PlayerRunState.h"
#include "Src/Actor/Character/Player/PlayerJumpState.h"
#include "Src/Actor/Character/Player/PlayerFallState.h"
#include "Src/Actor/Character/Player/PlayerTutorialPauseStage.h"


#include "Src/Core/BossUIManager.h"

// プレイヤーステータス構造体。
struct PlayerStatus
{
    // 無敵時間。
    static constexpr auto INVINCIBLE_TIME = 1.0f; // 無敵時間。
    // 体力。
    static const uint8_t MAX_HP = 9; // 初期体力。

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


bool Player::Start()
{
    // ステートの登録。
    RegisterState<app::state::PlayerIdleState>(enState_Idle);
    RegisterState<app::state::PlayerRunState>(enState_Run);
    RegisterState<app::state::PlayerJumpState >(enState_Jump);
    RegisterState<app::state::PlayerFallState >(enState_Fall);
    RegisterState<app::state::PlayerTutorialPauseStage>(enState_TutorialPause);


    // 初期ステートの設定。
    pCurrentState_ = pStateArray_[enState_Idle];
    pCurrentState_->Enter();

    // 移動速度。
    walkSpeed_ = PlayerStatus::Move::SPEED;

    // モデルの初期化。
    pRender_ = new Character2DRender();
    pRender_->Init(std::vector<std::string>(PARH_LIST, PARH_LIST + MAX_NUM));

    // 当たり判定。
    charaCon_.Init(2.0f, 1.0f, pos_);

    // プレイヤーのステータスを初期化する。
    // 最大HP、移動速度、攻撃力を設定。   
    status_.Initial(PlayerStatus::MAX_HP, walkSpeed_, PlayerStatus::ATTACK_POWER);

    score_ = 0;
    invincibleTime_ = 0.0f;

    // プレイヤーの座標を描画。
    posFont_.SetPosition({-600.0f, 300.0f, 0.0f});
    posFont_.SetScale(1.0f);                     // 文字の大きさ
    posFont_.SetColor({1.0f, 1.0f, 1.0f, 1.0f}); // 白文字

    // 壁クラスからチュートリアル遷移要求があるかチェック。
    if (requestTutorialPause_ && !isTutorialDone_)
    {
        isTutorialDone_ = true;

        
    }

    SetRespwanPos(pos_);

    // 踏みつけた際の跳ね返り値を設定。
    SetJumpPower(150.0f);

    return true;
}


void Player::Update()
{
    // フェード中は何もせずにreturn。
    Fade* fade = SceneManager::GetInstance()->GetFade();
    if (fade != nullptr && fade->GetFadeState() == FadeState::Fade_Out)
        return;


    if (pCurrentState_ != pStateArray_[enState_TutorialPause])
    {
        if (IsDimensionSwitchAction())
            app::core::InputManager::GetInstance()->FlipDimension(pCameraManager_);
    }

    // 無敵時間をカウント。
    if (invincibleTime_ > 0.0f)
        invincibleTime_ -= g_gameTime->GetFrameDeltaTime();

    // チュートリアルポーズ中は更新しない。
    if (isPaused_)
    {
        if (pCurrentState_ != pStateArray_[enState_TutorialPause])
           return;
    }

    _ASSERT(pCurrentState_ != nullptr);

    uint8_t request;

    // 壁クラスからチュートリアル遷移要求があるか確認。
    if (requestTutorialPause_ && !isTutorialDone_)
    {
        requestTutorialPause_ = false;
        request = enState_TutorialPause;

        // 遷移処理を実行。
        pCurrentState_->Exit();
        pCurrentState_ = pStateArray_[request];
        pCurrentState_->Enter();
        return;
    }


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
    bool is2Dmode = (pCameraManager_->GetCurrentCameraMode() == CameraMode::mode2D);
    charaCon_.Set2DMode(is2Dmode);

    // 移動処理。
    pos_ = charaCon_.Execute(moveSpeed_, ficedTime);

    // 移動制限の適応。
    AddMovementRestrictions();

    if (pCameraManager_ && pCameraManager_->GetCurrentCameraMode() == CameraMode::mode2D)
        pos_.z = 0.0f;

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


const auto line = -100.0f;
void Player::Render(RenderContext& rc)
{
    // キャラモデル。
    pRender_->Render(rc);

    // 座標表示。    
//    posFont_.Draw(rc);
}


void Player::CheckRespawn()
{
    // ステージ遷移中はダメージ処理を行わない。
    auto* pGame = FindGO<app::core::Game>("game");
    if (pGame && pGame->IsStageTransitioning())
        return;

    if (invincibleTime_ > 0.0f || status_.IsDead())
        return;

    if (pos_.y < line)
    {
        SetPlayerPos(respwanPos_);
        moveSpeed_ = Vector3::Zero;
        respawnFlag_ = true;
    }
}


void Player::OnDamage(uint8_t damage)
{
    if (invincibleTime_ > 0.0f || status_.IsDead())
        return;

    status_.Damage(static_cast<uint8_t>(damage));
    invincibleTime_ = PlayerStatus::INVINCIBLE_TIME;

    if (status_.IsDead())
    {
        SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
    }
}


bool Player::TryFlipDimension(bool area)
{
    // カメラがない、Bボタンが押されていないなら何もしない
    if (!pCameraManager_ || !g_pad[0]->IsTrigger(enButtonB))
        return false;

    //
    if (area || GetInTriggerArea())
    {
        auto currentMode = pCameraManager_->GetCurrentCameraMode();
        if (currentMode == CameraMode::mode2D || currentMode == CameraMode::mode3D)
        {
            app::core::InputManager::GetInstance()->FlipDimension(pCameraManager_);
            return true;
        }
    }

    return false;
}


void Player::AddMovementRestrictions()
{
    // 移動制限の適応。
    if (isMoveLimited_)
    {
        // X軸。
        if (pos_.x < moveLimitMin_.x)
            pos_.x = moveLimitMin_.x;

        if (pos_.x > moveLimitMax_.x)
            pos_.x = moveLimitMax_.x;


        // Y軸。
        if (pos_.y < moveLimitMin_.y)
            pos_.y = moveLimitMin_.y;


        // Z軸。
        if (pos_.z < moveLimitMin_.z)
            pos_.z = moveLimitMin_.z;

        if (pos_.z > moveLimitMax_.z)
            pos_.z = moveLimitMax_.z;
    }
}


void Player::CalculateMovement(const Vector3& stickInput)
{
    if (!pCameraManager_)
        return;

    // カメラの向きを取得する。
    Vector3 camRight = g_camera3D->GetRight();
    Vector3 camForward = g_camera3D->GetForward();

    // Y軸の影響を受けないようにする。
    camRight.y = 0.0f;
    camForward.y = 0.0f;

    // ベクトルを正規化する。
    camRight.Normalize();
    camForward.Normalize();

    // 視点による軸の補正（ここを修正）
    // 前方向ベクトルの Z成分 と X成分 の大きさを比べて、どちらがメインの軸か判定します
    if (fabsf(camForward.z) > fabsf(camForward.x))
    {
        // --- Z軸がメインの場合（通常の2Dモードや、正面奥に進む3D視点） ---

        // ForwardはZ軸のみにする（Xを消す）
        camForward.x = 0.0f;
        camForward.z = (camForward.z > 0.0f) ? 1.0f : -1.0f;

        // RightはX軸のみにする（Zを消す）
        camRight.z = 0.0f;
        camRight.x = (camRight.x > 0.0f) ? 1.0f : -1.0f;
    }
    else
    {
        // --- X軸がメインの場合（横から見る視点など） ---

        // ForwardはX軸のみにする（Zを消す）
        camForward.z = 0.0f;
        camForward.x = (camForward.x > 0.0f) ? 1.0f : -1.0f;

        // RightはZ軸のみにする（Xを消す）
        camRight.x = 0.0f;
        camRight.z = (camRight.z > 0.0f) ? 1.0f : -1.0f;
    }

    // 移動速度の計算
    moveSpeed_.x = (camRight.x * stickInput.x + camForward.x * stickInput.z) * walkSpeed_;
    moveSpeed_.z = (camRight.z * stickInput.x + camForward.z * stickInput.z) * walkSpeed_;
}
