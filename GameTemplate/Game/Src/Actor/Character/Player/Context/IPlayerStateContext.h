#pragma once
#include "Src/Actor/Character/Player/Character2DRender.h"

class CameraManager;

namespace nsApp
{
    /**
     * @brief Player ステートがアクセスしてよい操作の集合。
     *
     * Player 本体の private メンバ（charaCon_ / pRender_ 等）を隠蔽し、
     * friend 宣言を不要にする。Boss / 雑魚敵用は別途 IBossStateContext 等を作る。
     */
    class IPlayerStateContext
    {
    public:
        virtual ~IPlayerStateContext() = default;

        // --- Locomotion（移動）---
        /** @brief 地面に接地しているか。 */
        virtual bool IsOnGround() const = 0;
        /** @brief 移動速度を取得する。 */
        virtual Vector3& GetMoveSpeed() = 0;
        /** @brief walkSpeedの取得。 */
        virtual float GetWalkSpeed() const = 0;
        /** @brief ジャンプ力の取得。 */
        virtual const float& GetJumpPower() const = 0;
        /** @brief 移動処理を加える。 */
        virtual void ApplyMovement() = 0;
        /** @brief プレイヤーの座標の取得。 */
        virtual Vector3& GetPlayerPos() = 0;
        /** @brief 座標。 */
        virtual void SetPlayerPos(const Vector3& pos) = 0;
        /** @brief キャラクターコントローラーのゲッター */
        virtual CharacterController& GetCharacterController() = 0;
        /** @brief カメラの向きに合わせて移動ベクトルの計算を行う。 */
        virtual void CalculateMovement(const Vector3& stickInput) = 0;

        // --- Presentation（描画）---
        /** @brief 現在のモデルインデックスをセット。 */
        virtual void SetCurrentIndex(uint8_t index) = 0;
        /**
         * @brief 座標と描画の同期。
         * @note 各 State 末尾の pRender_->SetPosition + Update をこれ1つに置き換える。
         */
        virtual void SyncView() = 0;

        // --- 入力・フラグ ---
        /** @brief キー入力保持のため設定 */
        virtual void SetKeyDirection(const Vector3& direction) = 0;
        /** @brief キー入力方向の取得。 */
        virtual Vector3 GetKeyDirection() const = 0;
        /** @brief 空中での操作が可能かどうかの設定。 */
        virtual void SetCanAirControl(bool flag) = 0;
        /** @brief 今フレームでジャンプしたかどうかの設定。 */
        virtual void SetJumpedThisFrame(bool flag) = 0;
        /** @brief リスポーンフラグのセット。 */
        virtual void SetRespawnFlag(bool flag) = 0;
        /** @brief リスポーンしたか。 */
        virtual bool IsRespawn() = 0;
        /** @brief バウンドフラグのセット。 */
        virtual void SetIsBounce(bool isBounce) = 0;
        /** @brief バウンドフラグ。 */
        virtual bool IsBounce() const = 0;

        // --- カメラ・次元 ---
        /** @brief カメラモードを取得。 */
        virtual CameraManager* GetCameraManager() = 0;
        /** @brief 視点切替の試行。 */
        virtual bool TryFlipDimension(bool area) = 0;

        // --- チュートリアル ---
        /** @brief チュートリアルが終わったかどうか。 */
        virtual bool IsTutorialDone() const = 0;
        /** @brief チュートリアル一時停止要求。 */
        virtual void RequestTutorialPause() = 0;

        // --- リスポーン・ダメージ ---
        /** @brief リスポーン座標の取得。 */
        virtual const Vector3& GetRespwanPos() const = 0;
        /** @brief リスポーン時の回転軸の取得。 */
        virtual const Quaternion& GetRespwanRot() const = 0;
        /** @brief 回転軸。 */
        virtual void SetRotation(const Quaternion& rot) = 0;
        /** @brief ダメージ処理。 */
        virtual void OnDamage(uint8_t damage) = 0;
    };

}
