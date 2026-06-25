#pragma once
#include "Src/Actor/Character/Player/Character2DRender.h"
#include "Src/Actor/Character/Status.h"
#include "Src/Actor/Character/IState.h"
#include "Src/Actor/Character/Character.h"

#include "Src/Core/CameraManager.h"

#include "Src/Actor/Character/Player/State/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/PlayerFallState.h"
#include "Src/Actor/Character/Player/State/PlayerTutorialPauseStage.h"

#include "Src/Actor/Character/Player/Context/IPlayerStateContext.h"
#include "Src/Actor/Character/Player/Component/PlayerLocomotion.h"
#include "Src/Actor/Character/Player/Component/PlayerPresentation.h"
#include "Src/Actor/Character/Player/Component/PlayerStateMachine.h"
#include "Src/StateMachine/StateMachineCommand.h"

enum PlayerState : uint8_t
{
    sIdle,
    sRun,
    sJump,
    sNum,
};


enum EnAnimationClip : uint8_t
{
    animIdle,
    animJump,
    animRun,
    animNum,
};

using EnPlayerState = nsApp::nsActor::nsCharacter::nsPlayer::EnPlayerState;

class IState;
class CameraManager;

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                /**
                 * @brief プレイヤーキャラクター。
                 *
                 * IPlayerStateContext を実装し、State は Context 経由でアクセスする。
                 * 移動・描画は Locomotion / Presentation コンポーネントに委譲する。
                 */
                class Player : public Character, public IPlayerStateContext
                {
                private:
                    CameraManager* pCameraManager_ = nullptr; ///< カメラマネージャ。
                    Character2DRender* pRender_ = nullptr;    ///< 2D 描画コンポーネント。
                    app::status::Status status_;              ///< ステータス。
                    nsApp::nsFunction::StateMachineCommand<EnPlayerState, enState_Num> stateCommand_;                                                       ///< ステートレジストリ。
                    PlayerStateMachine stateMachine_; ///< ステートマシン。
                    PlayerLocomotion locomotion_;     ///< 移動コンポーネント。
                    PlayerPresentation presentation_; ///< 描画コンポーネント。

                public:
                    AnimationClip animationClip_[EnAnimationClip::animNum]; ///< アニメーションクリップ配列。
                    CharacterController charaCon_;                          ///< キャラクターコントローラ。
                    Vector3 diff_ = Vector3::Zero;                          ///< 差分。
                    Vector3 moveSpeed_ = Vector3::Zero;                     ///< 移動速度。
                    Vector3 pos_ = Vector3::Zero;                           ///< 座標。
                    Vector3 respwanPos_ = Vector3::Zero;                    ///< リスポーン座標。
                    Vector3 forward_ = Vector3::Front;                      ///< 前方向。
                    Vector3 keyDirection_ = Vector3::Zero;                  ///< キー入力方向。
                    Vector3 moveLimitMin_ = Vector3::Zero;                  ///< 移動制限最小。
                    Vector3 moveLimitMax_ = Vector3::Zero;                  ///< 移動制限最大。
                    Quaternion rot_ = Quaternion::Identity;                 ///< 回転。
                    Quaternion respwanRot_ = Quaternion::Identity;          ///< リスポーン時回転。
                    Quaternion offsetRot_ = Quaternion::Identity;           ///< カメラオフセット回転。
                    FontRender posFont_;                                    ///< 座標表示フォント。
                private:
                    uint8_t state_;                     ///< 旧 PlayerState 互換用。
                    uint8_t triggerOverlapCount_ = 0;   ///< トリガーエリア重なり数。
                    uint8_t currentIndex = 0;           ///< 現在のモデルインデックス。
                    int score_ = 0;                     ///< スコア。
                    float invincibleTime_ = 0.0f;       ///< 無敵時間。
                    float jumpPower_ = 0.0f;            ///< ジャンプ力。
                    float walkSpeed_ = 0.0f;            ///< 歩行速度。
                    float bounceCooldown_ = 0.0f;       ///< バウンドクールダウン。
                    bool canAirControl_ = false;        ///< 空中操作可能か。
                    bool didJumpThisFrame_ = false;     ///< 今フレームでジャンプしたか。
                    bool isPaused_ = false;             ///< 一時停止中か。
                    bool is3DMode_ = false;             ///< 3D モードか。
                    bool respawnFlag_ = false;          ///< リスポーンフラグ。
                    bool isCutInActive;                 ///< カットイン中か。
                    bool isMoveLimited_ = false;        ///< 移動制限が有効か。
                    bool isTutorialDone_ = false;       ///< チュートリアル完了か。
                    bool requestTutorialPause_ = false; ///< チュートリアル一時停止要求。
                    bool isBounce_ = false;             ///< バウンド中か。
                public:
                    /**
                     * @brief コンストラクタ。
                     */
                    Player() : stateMachine_(stateCommand_, *this) {}
                    virtual ~Player() = default;

                public:
                    bool Start() override;
                    void Update() override;
                    void Render(RenderContext& rendercontext) override;
                    /**
                     * @brief 次元切替アクションが発生したか判定する。
                     * @return 切替する場合 true。
                     */
                    bool IsDimensionSwitchAction();
                    /**
                     * @brief 敵を踏みつけた際のバウンド処理。
                     */
                    void Bound()
                    {
                        /** @brief 二重バウンド防止。 */
                        if (moveSpeed_.y > 0.0f)
                            return;
                        if (bounceCooldown_ > 0.0f)
                            return;
                        isBounce_ = true;
                        stateMachine_.ChangeState(enState_Jump);
                        state_ = PlayerState::sJump;
                        canAirControl_ = true;
                        bounceCooldown_ = 0.2f;
                    }
                    /**
                     * @brief リスポーン処理。
                     */
                    void CheckRespawn();
                    /**
                     * @brief 視点切替の試行。
                     * @param[in] area エリア内からの要求か。
                     * @return 切替した場合 true。
                     */
                    bool TryFlipDimension(bool area) override;

                public:
                    /**
                     * @brief 移動速度を加算する。
                     * @param[in] addMoveSpeed 加算する速度。
                     */
                    inline void AddMoveSpeed(const Vector3& addMoveSpeed) { moveSpeed_ += addMoveSpeed; }
                    /**
                     * @brief スコアを加算する。
                     * @param[in] amount 加算量。
                     */
                    inline void AddScore(int amount) { score_ += amount; }
                    /**
                     * @brief カメラの向きに合わせて移動ベクトルを計算する。
                     * @param[in] stickInput スティック入力。
                     */
                    void CalculateMovement(const Vector3& stickInput) override;

                public:
                    /**
                     * @brief CameraManager を初期化する。
                     * @param[in] pCameraManager カメラマネージャ。
                     */
                    inline void InitCameraManager(CameraManager* pCameraManager) { pCameraManager_ = pCameraManager; }
                    /**
                     * @brief ジャンプ力を設定する。
                     * @param[in] jumpPower ジャンプ力。
                     */
                    inline void SetJumpPower(float jumpPower) { jumpPower_ = jumpPower; }
                    /**
                     * @brief トリガーエリア進入時に呼ぶ。
                     */
                    inline void EnterTriggerArea() { triggerOverlapCount_++; }
                    /**
                     * @brief トリガーエリア退出時に呼ぶ。
                     */
                    inline void ExitTriggerArea()
                    {
                        triggerOverlapCount_--;
                        if (triggerOverlapCount_ < 0)
                            triggerOverlapCount_ = 0;
                    }
                    /**
                     * @brief 座標を設定する。
                     * @param[in] pos 座標。
                     */
                    void SetPlayerPos(const Vector3& pos) override;
                    /**
                     * @brief 一時停止フラグを設定する。
                     * @param[in] isPaused 一時停止する場合 true。
                     */
                    inline void SetPaused(bool isPaused) { isPaused_ = isPaused; }
                    /**
                     * @brief リスポーン座標を設定する。
                     * @param[in] pos 座標。
                     */
                    inline void SetRespwanPos(const Vector3& pos) { respwanPos_ = pos; }
                    /**
                     * @brief 空中操作可能フラグを設定する。
                     * @param[in] flag 空中操作可能なら true。
                     */
                    void SetCanAirControl(bool flag) override;
                    /**
                     * @brief 今フレームでジャンプしたかを設定する。
                     * @param[in] flag ジャンプした場合 true。
                     */
                    void SetJumpedThisFrame(bool flag) override;
                    /**
                     * @brief 回転を設定する。
                     * @param[in] rot 回転。
                     */
                    void SetRotation(const Quaternion& rot) override;
                    /**
                     * @brief リスポーンフラグを設定する。
                     * @param[in] flag リスポーンした場合 true。
                     */
                    void SetRespawnFlag(bool flag) override;
                    /**
                     * @brief 現在のモデルインデックスを設定する。
                     * @param[in] index インデックス。
                     */
                    void SetCurrentIndex(uint8_t index) override;
                    /**
                     * @brief キー入力方向を設定する。
                     * @param[in] direction 入力方向。
                     */
                    void SetKeyDirection(const Vector3& direction) override;
                    /**
                     * @brief 移動制限を設定する。
                     * @param[in] min 最小座標。
                     * @param[in] max 最大座標。
                     */
                    inline void SetMoveLimit(const Vector3& min, const Vector3& max)
                    {
                        moveLimitMin_ = min;
                        moveLimitMax_ = max;
                        isMoveLimited_ = true;
                    }
                    /**
                     * @brief 移動制限を解除する。
                     */
                    inline void ReleaseMoveLimit() { isMoveLimited_ = false; }
                    /**
                     * @brief チュートリアル完了フラグを設定する。
                     * @param[in] isDone 完了している場合 true。
                     */
                    inline void SetTutorialFlag(bool isDone) { isTutorialDone_ = isDone; }
                    /**
                     * @brief チュートリアル一時停止を要求する。
                     */
                    void RequestTutorialPause() override;
                    /**
                     * @brief チュートリアル完了を設定する。
                     * @param[in] isDone 完了している場合 true。
                     */
                    inline void SetTutorialDone(bool isDone) { isTutorialDone_ = isDone; }
                    /**
                     * @brief バウンドフラグを設定する。
                     * @param[in] isBounce バウンド中なら true。
                     */
                    void SetIsBounce(bool isBounce) override;

                public:
                    /**
                     * @brief スコアを取得する。
                     * @return スコア。
                     */
                    inline int GetScore() { return score_; }
                    /**
                     * @brief HP を取得する。
                     * @return 現在 HP。
                     */
                    inline uint8_t GetHP() const { return status_.GetHP(); }
                    /**
                     * @brief 最大 HP を取得する。
                     * @return 最大 HP。
                     */
                    inline uint8_t GetMaxHP() const { return status_.GetMaxHP(); }
                    /**
                     * @brief ジャンプ力を取得する。
                     * @return ジャンプ力。
                     */
                    const float& GetJumpPower() const override { return jumpPower_; }
                    /**
                     * @brief 座標を取得する。
                     * @return 座標への参照。
                     */
                    Vector3& GetPlayerPos() override { return pos_; }
                    /**
                     * @brief 前方向ベクトルを取得する。
                     * @return 前方向。
                     */
                    inline const Vector3 GetForward() const { return forward_; }
                    /**
                     * @brief トリガーエリア内かどうか。
                     * @return エリア内なら true。
                     */
                    inline bool GetInTriggerArea() const { return triggerOverlapCount_ > 0; }
                    /**
                     * @brief カメラマネージャを取得する。
                     * @return カメラマネージャ。
                     */
                    CameraManager* GetCameraManager() override { return pCameraManager_; }
                    /**
                     * @brief リスポーンしたかどうか。
                     * @return リスポーンした場合 true。
                     */
                    bool IsRespawn() override { return respawnFlag_; }
                    /**
                     * @brief 歩行速度を取得する。
                     * @return 歩行速度。
                     */
                    float GetWalkSpeed() const override { return walkSpeed_; }
                    /**
                     * @brief 今フレームでジャンプしたか。
                     * @return ジャンプした場合 true。
                     */
                    inline bool DoJumpCheck() const { return didJumpThisFrame_; }
                    /**
                     * @brief リスポーン座標を取得する。
                     * @return リスポーン座標。
                     */
                    const Vector3& GetRespwanPos() const override { return respwanPos_; }
                    /**
                     * @brief リスポーン時の回転を取得する。
                     * @return 回転。
                     */
                    const Quaternion& GetRespwanRot() const override { return respwanRot_; }
                    /**
                     * @brief キー入力方向を取得する。
                     * @return 入力方向。
                     */
                    Vector3 GetKeyDirection() const override { return keyDirection_; }
                    /**
                     * @brief キャラクターコントローラを取得する。
                     * @return キャラクターコントローラ。
                     */
                    CharacterController& GetCharacterController() override { return charaCon_; }
                    /**
                     * @brief 移動速度を取得する。
                     * @return 移動速度への参照。
                     */
                    Vector3& GetMoveSpeed() override { return moveSpeed_; }
                    /**
                     * @brief 2D 描画コンポーネントを取得する。
                     * @return 描画コンポーネント。
                     */
                    inline Character2DRender* GetCharacter2DRender() { return pRender_; }
                    /**
                     * @brief プレイヤーの向きを取得する。
                     * @return 回転。
                     */
                    inline Quaternion GetPlayerRotation() const
                    {
                        pRender_->GetRotation();
                        return rot_;
                    }
                    /**
                     * @brief 一時停止中かどうか。
                     * @return 一時停止中なら true。
                     */
                    inline bool IsPaused() const { return isPaused_; }
                    /**
                     * @brief チュートリアルを完了したか。
                     * @return 完了している場合 true。
                     */
                    inline bool IsTutorialComplete() const { return isTutorialDone_; }
                    /**
                     * @brief チュートリアルが終わったかどうか。
                     * @return 終了している場合 true。
                     */
                    bool IsTutorialDone() const override { return isTutorialDone_; }
                    /**
                     * @brief バウンド中かどうか。
                     * @return バウンド中なら true。
                     */
                    bool IsBounce() const override { return isBounce_; }


                public:
                    // --- IPlayerStateContext（Locomotion / Presentation 委譲）---
                    /**
                     * @brief 地面に接地しているか。
                     * @return 接地していれば true。
                     */
                    bool IsOnGround() const override;
                    /**
                     * @brief 移動処理を加える。
                     */
                    void ApplyMovement() override;
                    /**
                     * @brief 座標と描画を同期する。
                     */
                    void SyncView() override;
                    /**
                     * @brief ダメージ処理。
                     * @param[in] damage ダメージ量。
                     */
                    void OnDamage(uint8_t damage) override;

                private:
                    /**
                     * @brief Context 化前の State が private メンバへ直接アクセスするための friend 宣言。
                     * @note Run / Jump / Fall / Tutorial を Context 化したら削除する。
                     */
                    friend class nsApp::nsState::PlayerRunState;
                    friend class nsApp::nsState::PlayerJumpState;
                    friend class nsApp::nsState::PlayerFallState;
                    friend class nsApp::nsState::PlayerTutorialPauseStage;
                };
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
}

