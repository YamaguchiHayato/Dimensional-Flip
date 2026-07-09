#include "stdafx.h"

#include "Src/Actor/Character/Player/Character2DRender.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/State/PlayerFallState.h"
#include "Src/Actor/Character/Player/State/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/PlayerTutorialPauseStage.h"
#include "Src/Actor/Character/Status.h"
#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Game.h"
#include "Src/Core/InputManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Production/CutIn/CutInView.h"
#include "Src/Production/Fade.h"

#include "Src/Parameter/Player/PlayerStatusParameterTable.h"
#include "Src/Parameter/Player/PlayerMoveParameterTable.h"
#include "Src/Parameter/Player/PlayerAirParameterTable.h"
#include "Src/Parameter/Player/PlayerPhysicsParameterTable.h"



/**
 * @brief プレイヤーステータス定数。
 */
struct PlayerStatus
{
    static constexpr auto INVINCIBLE_TIME = 1.0f; ///< 無敵時間。
    static const uint8_t MAX_HP = 9;              ///< 初期体力。
    static constexpr auto ATTACK_POWER = 1.0f;    ///< 攻撃力。

    /**
     * @brief 移動パラメータ。
     */
    struct Move
    {
        static constexpr float SPEED = 100.0f; ///< 移動速度。
    };

    /**
     * @brief ジャンプパラメータ。
     */
    struct Jump
    {
        static constexpr float GLAVITY = 2.5f; ///< 落下中の重力倍率。
        static constexpr float CUT = 2.5f;     ///< ボタン離し後の重力倍率。
    };
};

namespace
{
    const Vector3 SCALE = Vector3(0.125f, 0.125f, 0.125f); ///< モデル表示スケール。
    const uint8_t MAX_NUM = 8;                             ///< モデル数。

    /**
     * @brief モデルパスリスト。
     */
    const std::string PARH_LIST[MAX_NUM] = {
        "Assets/modelData/girl/facing_left/L_idle.tkm",
        "Assets/modelData/girl/facing_left/L_jump.tkm",
        "Assets/modelData/girl/facing_left/L_run_front_legRight.tkm",
        "Assets/modelData/girl/facing_left/L_run_front_legLeft.tkm",
        "Assets/modelData/girl/facing_right/R_idle.tkm",
        "Assets/modelData/girl/facing_right/R_jump.tkm",
        "Assets/modelData/girl/facing_right/R_run_front_legRight.tkm",
        "Assets/modelData/girl/facing_right/R_run_front_legLeft.tkm",
    };

    static constexpr float DEAD_LINE_Y = -100.0f; ///< 落下死亡ライン。
} // namespace


namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                /**
                 * @brief 開始処理。
                 */
                bool Player::Start()
                {
                    /** @brief モデル初期化（Bind より先）。 */
                    pRender_ = new Character2DRender();
                    pRender_->Init(std::vector<std::string>(PARH_LIST, PARH_LIST + MAX_NUM));

                    /** @brief コンポーネントのバインド。 */
                    locomotion_.Bind(charaCon_, pos_, moveSpeed_, walkSpeed_, moveLimitMin_, moveLimitMax_, isMoveLimited_, pCameraManager_, pRender_);

                    /** @brief 描画のバインド。 */
                    presentation_.Bind(pRender_, currentIndex, rot_, offsetRot_, SCALE);

                    /** @brief ステート登録。 */
                    stateCommand_.Emplace<nsApp::nsState::PlayerIdleState>(enState_Idle, static_cast<nsApp::IPlayerStateContext&>(*this));
                    stateCommand_.Emplace<nsApp::nsState::PlayerRunState>(enState_Run, this);
                    stateCommand_.Emplace<nsApp::nsState::PlayerJumpState>(enState_Jump, this);
                    stateCommand_.Emplace<nsApp::nsState::PlayerFallState>(enState_Fall, this);
                    stateCommand_.Emplace<nsApp::nsState::PlayerTutorialPauseStage>(enState_TutorialPause, static_cast<Player*>(this));
                    /** @brief 初期ステート開始。 */
                    stateMachine_.Start(enState_Idle);

                    walkSpeed_ = PlayerStatus::Move::SPEED;

                    /** @brief TSV パラメータの取得。 */
                    using namespace nsApp::nsSystem;
                    const auto& statusParam = PlayerStatusParameterTable::Get();
                    const auto& moveParam = PlayerMoveParameterTable::Get();
                    const auto& airParam = PlayerAirParameterTable::Get();
                    const auto& physicsParam = PlayerPhysicsParameterTable::Get();

                    walkSpeed_ = moveParam.walkSpeed;
                    SetJumpPower(airParam.jumpPower);
                    charaCon_.Init(physicsParam.colliderRadius, physicsParam.colliderHeight, pos_);
                    locomotion_.SetFixedDeltaTime(physicsParam.fixedDeltaTime);
                    status_.Initial(statusParam.maxHP, walkSpeed_, statusParam.attackPower);
                    score_ = 0;
                    invincibleTime_ = 0.0f;
                    posFont_.SetPosition({-600.0f, 300.0f, 0.0f});
                    posFont_.SetScale(1.0f);
                    posFont_.SetColor({1.0f, 1.0f, 1.0f, 1.0f});


                    if (requestTutorialPause_ && !isTutorialDone_)
                        isTutorialDone_ = true;
                    SetRespwanPos(pos_);

                    return true;
                }

                /**
                 * @brief 更新処理。
                 */
                void Player::Update()
                {
                    if (bounceCooldown_ > 0.0f)
                        bounceCooldown_ -= g_gameTime->GetFrameDeltaTime();

                    /** @brief チュートリアルポーズ中以外は次元切替を許可。 */
                    if (!stateMachine_.IsInState(enState_TutorialPause))
                    {
                        if (IsDimensionSwitchAction())
                            app::core::InputManager::GetInstance()->FlipDimension(pCameraManager_);
                    }

                    if (invincibleTime_ > 0.0f)
                        invincibleTime_ -= g_gameTime->GetFrameDeltaTime();

                    /** @brief チュートリアル遷移要求。 */
                    if (requestTutorialPause_ && !isTutorialDone_)
                    {
                        requestTutorialPause_ = false;
                        stateMachine_.ChangeState(enState_TutorialPause);
                        return;
                    }

                    /** @brief ステートマシン更新（フェード・ポーズは PlayerStateMachine::CanUpdate）。 */
                    stateMachine_.Update();

                    /** @brief 描画フレーム更新。 */
                    presentation_.PostFrameUpdate();

                    didJumpThisFrame_ = false;

                    presentation_.UpdateOffsetRotation(pCameraManager_);
                    CheckRespawn();
                    presentation_.ApplyRotation();

                    wchar_t text[256];
                    swprintf_s(text, L"Player Pos\nX: %.1f\nY: %.1f\nZ: %.1f", pos_.x, pos_.y, pos_.z);
                    posFont_.SetText(text);
                }

                /**
                 * @brief 次元切替アクションが発生したか判定する。
                 */
                bool Player::IsDimensionSwitchAction()
                {
                    if (!pCameraManager_)
                        return false;

                    const bool isInTriggerArea = GetInTriggerArea();
                    const auto currentMode = pCameraManager_->GetCurrentCameraMode();

                    if (isInTriggerArea && g_pad[0]->IsTrigger(enButtonB))
                    {
                        if (currentMode == CameraMode::mode2D || currentMode == CameraMode::mode3D)
                            return true;
                    }

                    return false;
                }

                /**
                 * @brief 地面に接地しているか。
                 */
                bool Player::IsOnGround() const
                {
                    return locomotion_.IsOnGround();
                }

                /**
                 * @brief 移動処理を加える。
                 */
                void Player::ApplyMovement()
                {
                    locomotion_.ApplyMovement();
                }

                /**
                 * @brief ステージ Restart 用に移動状態をリセットする。
                 */
                void Player::ResetForStageRestart()
                {
                    moveSpeed_ = Vector3::Zero;
                    isBounce_ = false;
                    SetRespawnFlag(false);
                    stateMachine_.ChangeState(enState_Idle);
                    charaCon_.SetPosition(pos_);
                }

                /**
                 * @brief 座標と描画を同期する。
                 */
                void Player::SyncView()
                {
                    presentation_.SyncView(pos_);
                }

                /**
                 * @brief 座標を設定する。
                 */
                void Player::SetPlayerPos(const Vector3& pos)
                {
                    pos_ = pos;
                    charaCon_.SetPosition(pos_);

                    if (pRender_)
                        pRender_->SetPosition(pos_);
                }

                /**
                 * @brief 空中操作可能フラグを設定する。
                 */
                void Player::SetCanAirControl(bool flag)
                {
                    canAirControl_ = flag;
                }

                /**
                 * @brief 今フレームでジャンプしたかを設定する。
                 */
                void Player::SetJumpedThisFrame(bool flag)
                {
                    didJumpThisFrame_ = flag;
                }

                /**
                 * @brief 回転を設定する。
                 */
                void Player::SetRotation(const Quaternion& rot)
                {
                    rot_ = rot;
                    if (pRender_)
                        pRender_->SetRotation(rot_);
                }

                /**
                 * @brief リスポーンフラグを設定する。
                 */
                void Player::SetRespawnFlag(bool flag)
                {
                    respawnFlag_ = flag;
                }

                /**
                 * @brief 現在のモデルインデックスを設定する。
                 */
                void Player::SetCurrentIndex(uint8_t index)
                {
                    currentIndex = index;
                }

                /**
                 * @brief キー入力方向を設定する。
                 */
                void Player::SetKeyDirection(const Vector3& direction)
                {
                    keyDirection_ = direction;
                }

                /**
                 * @brief チュートリアル一時停止を要求する。
                 */
                void Player::RequestTutorialPause()
                {
                    requestTutorialPause_ = true;
                }

                /**
                 * @brief バウンドフラグを設定する。
                 */
                void Player::SetIsBounce(bool isBounce)
                {
                    isBounce_ = isBounce;
                }

                /**
                 * @brief カメラの向きに合わせて移動ベクトルを計算する。
                 */
                void Player::CalculateMovement(const Vector3& stickInput)
                {
                    locomotion_.CalculateMovement(stickInput);
                }

                /**
                 * @brief 描画処理。
                 */
                void Player::Render(RenderContext& rc)
                {
                    presentation_.Render(rc);
                }

                /**
                 * @brief リスポーン処理。
                 */
                void Player::CheckRespawn()
                {
                    auto* pGame = FindGO<app::core::Game>("game");
                    if (pGame && pGame->IsStageTransitioning())
                        return;

                    if (invincibleTime_ > 0.0f || status_.IsDead())
                        return;

                    if (pos_.y < DEAD_LINE_Y)
                    {
                        SetPlayerPos(respwanPos_);
                        moveSpeed_ = Vector3::Zero;
                        respawnFlag_ = true;
                    }
                }

                /**
                 * @brief ダメージ処理。
                 */
                void Player::OnDamage(uint8_t damage)
                {
                    if (invincibleTime_ > 0.0f || status_.IsDead())
                        return;

                    status_.Damage(static_cast<uint8_t>(damage));
                    invincibleTime_ = PlayerStatus::INVINCIBLE_TIME;

                    if (status_.IsDead())
                        SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
                }

                /**
                 * @brief 視点切替の試行。
                 */
                bool Player::TryFlipDimension(bool area)
                {
                    if (!pCameraManager_ || !g_pad[0]->IsTrigger(enButtonB))
                        return false;

                    if (area || GetInTriggerArea())
                    {
                        const auto currentMode = pCameraManager_->GetCurrentCameraMode();
                        if (currentMode == CameraMode::mode2D || currentMode == CameraMode::mode3D)
                        {
                            app::core::InputManager::GetInstance()->FlipDimension(pCameraManager_);
                            return true;
                        }
                    }

                    return false;
                }
            }
        }
    }
}

