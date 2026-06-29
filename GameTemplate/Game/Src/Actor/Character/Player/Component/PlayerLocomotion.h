#pragma once
#include "Src/Actor/Character/Player/Character2DRender.h"
#include "Src/Parameter/Player/PlayerAirParameterTable.h"

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
                 * @brief Player の移動・物理を担当するコンポーネント。
                 *
                 * charaCon_ / moveSpeed_ / pos_ 等の実データは Player が所有し、
                 * このクラスは処理だけを集約する。
                 * カメラ向きベクトルや 2D モード判定などの作業用データもメンバに保持する。
                 */
                class PlayerLocomotion
                {
                public:
                    /* コンストラクタ。*/
                    PlayerLocomotion() = default;

                    /**
                     * @brief Player 側のメンバをバインドする（Start 時に1回呼ぶ）。
                     * @param[in,out] charaCon       キャラクターコントローラ
                     * @param[in,out] pos            座標
                     * @param[in,out] moveSpeed      移動速度
                     * @param[in,out] walkSpeed      地上歩行速度
                     * @param[in,out] moveLimitMin   移動制限の最小座標
                     * @param[in,out] moveLimitMax   移動制限の最大座標
                     * @param[in,out] isMoveLimited  移動制限が有効か
                     * @param[in,out] pCameraManager カメラマネージャへの参照
                     * @param[in,out] pRender        2D 描画コンポーネントへの参照
                     */
                    void Bind(CharacterController& charaCon,
                              Vector3& pos,
                              Vector3& moveSpeed,
                              float& walkSpeed,
                              Vector3& moveLimitMin,
                              Vector3& moveLimitMax,
                              bool& isMoveLimited,
                              CameraManager*& pCameraManager,
                              Character2DRender*& pRender);

                    /**
                     * @brief 空中重力を加える。
                     * @param speed 速度。
                     * @param isOnGround 地面に接地しているかどうかのフラグ。
                     * @param air 空中パラメータ。
                     * @param isBounce バウンドしたかどうかのフラグ。バウンドした場合は true にセットされる。
                     * @param allowJumpCut ジャンプカットを許可するかどうか。true
                     * の場合、上昇中にジャンプボタンを離すと上昇速度が減衰する。
                     */
                    void ApplyAirGravity(
                              Vector3& speed,
                              const nsSystem::PlayerAirParameter& air,
                              bool isOnGround,
                              bool& isBounce,
                              bool allowJumpCut);

                    /**
                     * @brief 移動処理を加える。
                     * @note キャラコン実行・移動制限・2D 時の Z 固定・座標反映まで行う。
                     */
                    void ApplyMovement();

                    /**
                     * @brief 移動制限を与える。
                     * @note moveLimitMin_ / moveLimitMax_ の範囲内に pos_ をクランプする。
                     */
                    void AddMovementRestrictions();

                    /**
                     * @brief カメラの向きに合わせて移動ベクトルの計算を行う。
                     * @param[in] stickInput スティック入力（x: 左右, z: 前後）
                     */
                    void CalculateMovement(const Vector3& stickInput);

                    /**
                     * @brief 地面に接地しているかどうかを返す。
                     * @return 接地していれば true
                     */
                    bool IsOnGround() const;

                    /**
                     * @brief キャラクターコントローラを取得する。
                     * @return CharacterController への参照
                     */
                    CharacterController& GetController();

                    /**
                     * @brief 移動速度を取得する。
                     * @return 移動速度への参照
                     */
                    Vector3& GetMoveSpeed();

                    /**
                     * @brief 座標を取得する。
                     * @return 座標への参照
                     */
                    Vector3& GetPosition();

                    /**
                     * @brief walkSpeedの取得。
                     * @return 地上歩行速度
                     */
                    float GetWalkSpeed() const;

                    /**
                     * @brief 物理更新の固定フレーム時間を設定する。
                     * @param[in] fixedDeltaTime 秒（例: 1/150）
                     */
                    inline void SetFixedDeltaTime(float fixedDeltaTime)
                    {
                        fixedDeltaTime_ = fixedDeltaTime;
                    }


                private:
                    /**
                     * @brief カメラの右方向・前方向を更新し、2.5D 用に軸をスナップする。
                     */
                    void UpdateCameraAxes();

                    /**
                     * @brief スナップ済みカメラ軸から移動速度 XZ を計算する。
                     * @param[in] stickInput スティック入力
                     */
                    void ApplyStickToMoveSpeed(const Vector3& stickInput);


                private:
                    // --- Player が所有するデータへの参照（Bind で接続）---
                    CharacterController* pCharaCon_ = nullptr;  //! キャラクターコントローラー。
                    Vector3* pPos_ = nullptr;                   //! 座標。
                    Vector3* pMoveSpeed_ = nullptr;             //! 移動速度。
                    Vector3* pMoveLimitMin_ = nullptr;          //! 移動制限の最小座標。
                    Vector3* pMoveLimitMax_ = nullptr;          //! 移動制限の最大座標。
                    float* pWalkSpeed_ = nullptr;               //! 地上歩行速度。
                    bool* pIsMoveLimited_ = nullptr;            //! 移動制限が有効か。
                    CameraManager* pCameraManager_ = nullptr;   //! カメラマネージャへのポインタの参照。
                    Character2DRender* pRender_ = nullptr;      //! 2D 描画コンポーネントへのポインタの参照。

                    // --- 移動計算の作業用メンバ---
                    Vector3 camRight_ = Vector3::Zero;          //! カメラの右方向（Y=0, 正規化済み）。
                    Vector3 camForward_ = Vector3::Zero;        //! カメラの前方向（Y=0, 正規化済み）。
                    bool is2DMode_ = false;                     //! 現在 2D カメラモードか。
                    float fixedDeltaTime_ = 0.0f;               //! 固定フレーム時間（1/150 秒）。
                    float lockedZ2D_ = 0.0f;                    //! 2Dモード中に固定するZ
                    bool was2DMode_ = false;                    //! 前フレームが2Dだったか
                };
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp
