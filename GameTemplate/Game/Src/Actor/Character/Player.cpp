#include "stdafx.h"


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


#include "Src/Actor/Character/Player/PlayerIdleState.h"
#include "Src/Actor/Character/Player/PlayerRunState.h"
#include "Src/Actor/Character/Player/PlayerJumpState.h"
#include "Src/Actor/Character/Player/PlayerFallState.h"

// プレイヤーステータス構造体。
struct PlayerStatus
{
    static const uint8_t MAX_HP = 8;               // 最大体力。
    static constexpr auto GLAVITY = 15.0f;         // 重力。
    static constexpr auto ATTACK_POWER = 1.0f;     // 攻撃力。

    // 移動速度パラメータ。
    struct Move
    {
        static constexpr float SPEED = 1125.0f;        // 移動速度アップ
        static constexpr float JUMP_POWER = 600.0f;    // ジャンプ力アップ
    };

    // ジャンプ用のパラメータ。
    struct Jump
    {
        static constexpr float GLAVITY = 2.5f;         // 落下中の重力倍率。
        static constexpr float CUT = 5.0f;             // ボタンを押した後の重力倍率。。
        static constexpr float FALLINGSPEED = -600.0f; // 落下速度。
    };
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


void Player::SetStageParam(bool isStageEX)
{
    if (isStageEX)
    {
        walkSpeed_ = PlayerStatus::Move::SPEED;
        jumpPower_ = PlayerStatus::Move::JUMP_POWER;
    }

    else
    {
        walkSpeed_ = PlayerStatus::Move::SPEED;
        jumpPower_ = PlayerStatus::Move::JUMP_POWER;
    }
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

    RegisterState<PlayerIdleState>(enState_Idle);
    RegisterState<PlayerRunState>(enState_Run);
    RegisterState<PlayerJumpState>(enState_Jump);
    RegisterState<PlayerFallState>(enState_Fall);

    pCurrentState_ = pStateArray_[enState_Idle];
    pCurrentState_->Enter();




    // アニメーションの設定。
    SetAnimation();
    render_.Init("Assets/modelData/unityChan.tkm", animationClip_, EnAnimationClip::animNum, enModelUpAxisY);
    charaCon_.Init(20.0f, 25.0f, pos_);

    // プレイヤーのステータスを初期化する。
    status_.Initial(PlayerStatus::MAX_HP, walkSpeed_, PlayerStatus::ATTACK_POWER);


    posFont_.SetPosition({-600.0f, 300.0f, 0.0f});
    posFont_.SetScale(1.0f);                     // 文字の大きさ
    posFont_.SetColor({1.0f, 1.0f, 1.0f, 1.0f}); // 白文字

    return true;
}


void Player::Update()
{
    if (isPaused_)
        return;

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
    /////////////////////////////////

    didJumpThisFrame_ = false;

//    Move();
    Action();
    Rotation();
    //PlayAnimation();
    //ManageState();
    render_.SetScale(SCALE);
    render_.SetPosition(pos_);
    render_.Update();




    wchar_t text[256];
    // %.1f は「小数点以下1桁まで表示」という意味です
    swprintf_s(text, L"Player Pos\nX: %.1f\nY: %.1f\nZ: %.1f", pos_.x, pos_.y, pos_.z);
    posFont_.SetText(text);
}

void Player::UpdateJumpAndGravity()
{
    const bool isGround = charaCon_.IsOnGround();

    // --- 地面上での処理 ---
    if (isGround)
    {
        // 地面にいる間は落下速度をリセット
        if (moveSpeed_.y < 0.0f)
            moveSpeed_.y = 0.0f;

        // Aボタンを押した瞬間だけジャンプ開始
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            moveSpeed_.y = jumpPower_;
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Jump);
            didJumpThisFrame_ = true;
        }
    }

    // 地上かつ完全停止中のときは重力をかけない（勝手に動かない）
    if (isGround && moveSpeed_.y <= 0.0f)
        return;

    if (moveSpeed_.y > 0.0f)
    {
        // 上昇中
        // ジャンプボタンを離したらジャンプを早めに切る（低いジャンプ）
        if (!g_pad[0]->IsPress(enButtonA))
            // ジャンプカット時は強めの重力
            moveSpeed_.y -= PlayerStatus::GLAVITY * PlayerStatus::Jump::CUT;

        else
            // 通常上昇中は普通の重力
            moveSpeed_.y -= PlayerStatus::GLAVITY;
    }
    else
        // 下降中：落下中の重力を強める
        moveSpeed_.y -= PlayerStatus::GLAVITY * PlayerStatus::Jump::GLAVITY;


    // 落下速度の制限（下向きがマイナス）
    if (moveSpeed_.y < PlayerStatus::Jump::FALLINGSPEED)
        moveSpeed_.y = PlayerStatus::Jump::FALLINGSPEED;
}


void Player::ReSpwan()
{
    pos_ = respwanPos_;
    rot_ = respwanRot_;

    render_.SetPosition(pos_);
    render_.SetRotation(rot_);
    charaCon_.SetPosition(pos_);

    // 移動速度のリセット。
    moveSpeed_ = Vector3::Zero;

    respawnFlag_ = true;
}


bool Player::IsDimensionSwitchAction()
{
    // カメラマネージャーを取得する。
    if (!pCameraManager_)
        return false;

    bool isInTriggerArea = GetInTriggerArea();
    auto currentMode = pCameraManager_->GetCurrentCameraMode();

    // カメラトリガー内かつ 2.5Dモードのときかつ Bボタンが押されたら。
    if (isInTriggerArea && currentMode == CameraMode::mode2_5D && g_pad[0]->IsTrigger(enButtonB))
    {
        return true;
    }
    return false;
}



void Player::ApplyMovement()
{
    // 移動処理。
    pos_ = charaCon_.Execute(moveSpeed_, 1.0f / 150.0f);

    // 座標のセット。
    charaCon_.SetPosition(pos_);
    render_.SetPosition(pos_);
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




void Player::Render(RenderContext& rc)
{
    render_.Draw(rc);
    posFont_.Draw(rc);
}

