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
#include "Src/Core/SoundManager.h"
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
        static constexpr float JUMP_POWER = 600.0f;   // ジャンプ力アップ
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
    didJumpThisFrame_ = false;

    Move();
    Action();
    Rotation();
    PlayAnimation();
    ManageState();
    render_.SetScale(SCALE);
    render_.SetPosition(pos_);
    render_.Update();




    wchar_t text[256];
    // %.1f は「小数点以下1桁まで表示」という意味です
    swprintf_s(text, L"Player Pos\nX: %.1f\nY: %.1f\nZ: %.1f", pos_.x, pos_.y, pos_.z);
    posFont_.SetText(text);
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
            {
                pCameraManager_->Request3DModeRot(0.0f);
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Flip);


            }

            else
            {
                pCameraManager_->Request3DModeRot(90.0f);
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Flip);

            }
        }
    }

    // Xボタンで元の2Dモード要求 (エリア内 かつ 3Dモード)
    else if (g_pad[0]->IsTrigger(enButtonX) && isInTriggerArea && currentMode == CameraMode::mode3D)
        pCameraManager_->Request2DMode();
}


void Player::Move()
{
    const bool GROUND = charaCon_.IsOnGround();

    if (GROUND)
        canAirControl_ = false;


    if (GROUND || canAirControl_)
    {
        // 移動する各成分を初期化する。
        moveSpeed_.x = 0.0f;
        moveSpeed_.z = 0.0f;

        // 移動処理。
        MoveHorizontal();
    }

    if (pos_.y <= -100.0f)
    {
        // リスポーン処理。
        ReSpwan();
        return;
    }

    // ジャンプ処理。
    UpdateJumpAndGravity();

    // 移動を与える処理。
    ApplyMovement();
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

    // ==== ここから空中処理 ====

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


//
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


void Player::MoveHorizontal()
{
    if (!pCameraManager_)
        return;

    // 現在のカメラモードの取得。
    CameraMode currentMode = pCameraManager_->GetCurrentCameraMode();

    // スティックの取得。
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    // デッドゾーン処理
    if (fabsf(stickL.x) < 0.2f)
        stickL.x = 0.0f;
    if (fabsf(stickL.y) < 0.2f)
        stickL.y = 0.0f;

    // 落下リスポーン処理
    if (pos_.y <= -200.0f)
    {
        ReSpwan();
        return;
    }

    Vector3 camRight = g_camera3D->GetRight();
    Vector3 camForward = g_camera3D->GetForward();
    camRight.y = 0.0f;
    camForward.y = 0.0f;
    camRight.Normalize();
    camForward.Normalize();

    if (fabsf(camForward.z) > fabsf(camForward.x))
    {
        // Z軸主体の移動。
        camForward.x = 0.0f;
        camForward.z = (camForward.z > 0.0f) ? 1.0f : -1.0f;

        // 右方向はx軸
        camRight.z = 0.0f;
        camRight.x = (camRight.x > 0.0f) ? 1.0f : -1.0f;
    }

    else
    {
        // X軸主体の向き（横向き）
        camForward.z = 0.0f;
        camForward.x = (camForward.x > 0.0f) ? 1.0f : -1.0f;

        // 右方向はZ軸になる
        camRight.x = 0.0f;
        camRight.z = (camRight.z > 0.0f) ? 1.0f : -1.0f;
    }

    bool isRotatedView = fabsf(camRight.z) > fabsf(camRight.x);

    if (currentMode == CameraMode::mode3D || currentMode == CameraMode::modeBoss || currentMode == CameraMode::modeStageEX || isRotatedView)
    {
        Vector3 targetMove = (camRight * stickL.x) + (camForward * stickL.y);
        moveSpeed_ += targetMove * walkSpeed_;
    }

    else
    {
        Vector3 targetMove = camRight * stickL.x;
        moveSpeed_ += targetMove * walkSpeed_;
    }
}



void Player::ApplyMovement()
{
    // 移動処理。
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
    posFont_.Draw(rc);
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
