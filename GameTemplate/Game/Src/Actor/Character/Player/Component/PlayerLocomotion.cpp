#include "stdafx.h"

#include "PlayerLocomotion.h"
#include "Src/Core/CameraManager.h"

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                void PlayerLocomotion::Bind(CharacterController& charaCon, Vector3& pos, Vector3& moveSpeed,
                                            float& walkSpeed, Vector3& moveLimitMin, Vector3& moveLimitMax,
                                            bool& isMoveLimited, CameraManager*& pCameraManager,
                                            Character2DRender*& pRender)
                {
                    pCharaCon_ = &charaCon;
                    pPos_ = &pos;
                    pMoveSpeed_ = &moveSpeed;
                    pWalkSpeed_ = &walkSpeed;
                    pMoveLimitMin_ = &moveLimitMin;
                    pMoveLimitMax_ = &moveLimitMax;
                    pIsMoveLimited_ = &isMoveLimited;
                    pCameraManager_ = pCameraManager;
                    pRender_ = pRender;

                    /** @brief 固定フレーム時間を初期化（Player::ApplyMovement と同値）。 */
                    fixedDeltaTime_ = 1.0f / 150.0f;
                }


                bool PlayerLocomotion::IsOnGround() const
                {
                    return pCharaCon_ != nullptr && pCharaCon_->IsOnGround();
                }


                CharacterController& PlayerLocomotion::GetController()
                {
                    return *pCharaCon_;
                }


                Vector3& PlayerLocomotion::GetMoveSpeed()
                {
                    return *pMoveSpeed_;
                }


                Vector3& PlayerLocomotion::GetPosition()
                {
                    return *pPos_;
                }


                float PlayerLocomotion::GetWalkSpeed() const
                {
                    return *pWalkSpeed_;
                }


                void PlayerLocomotion::ApplyAirGravity(Vector3& speed, const nsSystem::PlayerAirParameter& air,bool isOnGround, bool& isBounce, bool allowJumpCut)
                {
                    if (isOnGround && speed.y <= 0.0f)
                    {
                        speed.y = 0.0f;
                        isBounce = false;
                        return;
                    }

                    if (speed.y > 0.0f)
                    {
                        /** @brief 上昇中の処理。 */
                        if (isBounce)
                            speed.y -= air.gravityBase * air.jumpCutScale;

                        /** @brief 上昇中のジャンプカット処理。 */
                        else if (allowJumpCut && !g_pad[0]->IsPress(enButtonA))
                            speed.y -= air.gravityBase * air.jumpCutScale;

                        else
                            /** @brief 上昇中の重力加速度を適用。 */
                            speed.y -= air.gravityBase;
                    }
                    else
                    {
                        /** @brief 落下中の処理。 */
                        if (speed.y <= 0.0f)
                            isBounce = false;

                        /** @brief 落下中の重力加速度を適用。 */
                        speed.y -= air.gravityBase * air.jumpGravityScale;
                    }

                    /** @brief 落下速度の制限。 */
                    if (speed.y < air.maxFallSpeed)
                        speed.y = air.maxFallSpeed;
                }


                void PlayerLocomotion::ApplyMovement()
                {
                    if (pCharaCon_ == nullptr || pCameraManager_ == nullptr)
                        return;

                    const bool is2DMode = (pCameraManager_->GetCurrentCameraMode() == CameraMode::mode2D);

                    // 2Dに入った瞬間だけ、今のZをロック（0に潰さない）
                    if (is2DMode && !was2DMode_)
                        lockedZ2D_ = pPos_->z;

                    was2DMode_ = is2DMode;
                    is2DMode_ = is2DMode;
                    pCharaCon_->Set2DMode(is2DMode_);

                    // 2D中はZ方向に動かない
                    if (is2DMode_)
                        pMoveSpeed_->z = 0.0f;

                    *pPos_ = pCharaCon_->Execute(*pMoveSpeed_, fixedDeltaTime_);

                    AddMovementRestrictions();

                    // 旧: pPos_->z = 0.0f;  ← これがワープの原因
                    if (is2DMode_)
                        pPos_->z = lockedZ2D_;

                    pCharaCon_->SetPosition(*pPos_);

                    if (pRender_ != nullptr)
                        pRender_->SetPosition(*pPos_);
                }


                void PlayerLocomotion::AddMovementRestrictions()
                {
                    if (pIsMoveLimited_ == nullptr || !*pIsMoveLimited_)
                        return;

                    /** @brief X 軸。 */
                    if (pPos_->x < pMoveLimitMin_->x)
                        pPos_->x = pMoveLimitMin_->x;

                    if (pPos_->x > pMoveLimitMax_->x)
                        pPos_->x = pMoveLimitMax_->x;

                    /** @brief Y 軸。 */
                    if (pPos_->y < pMoveLimitMin_->y)
                        pPos_->y = pMoveLimitMin_->y;

                    /** @brief Z 軸。 */
                    if (pPos_->z < pMoveLimitMin_->z)
                        pPos_->z = pMoveLimitMin_->z;

                    if (pPos_->z > pMoveLimitMax_->z)
                        pPos_->z = pMoveLimitMax_->z;
                }


                void PlayerLocomotion::UpdateCameraAxes()
                {
                    /** @brief カメラの向きを取得する。 */
                    camRight_ = g_camera3D->GetRight();
                    camForward_ = g_camera3D->GetForward();

                    /** @brief Y 軸の影響を受けないようにする。 */
                    camRight_.y = 0.0f;
                    camForward_.y = 0.0f;

                    /** @brief ベクトルを正規化する。 */
                    camRight_.Normalize();
                    camForward_.Normalize();

                    /**
                     * @brief 視点による軸の補正。
                     * @note 前方向ベクトルの Z 成分と X 成分の大きさを比べ、メイン軸を判定する。
                     */
                    if (fabsf(camForward_.z) > fabsf(camForward_.x))
                    {
                        /** @brief Z 軸がメインの場合。 */
                        camForward_.x = 0.0f;
                        camForward_.z = (camForward_.z > 0.0f) ? 1.0f : -1.0f;

                        camRight_.z = 0.0f;
                        camRight_.x = (camRight_.x > 0.0f) ? 1.0f : -1.0f;
                    }
                    else
                    {
                        /** @brief X 軸がメインの場合。 */
                        camForward_.z = 0.0f;
                        camForward_.x = (camForward_.x > 0.0f) ? 1.0f : -1.0f;

                        camRight_.x = 0.0f;
                        camRight_.z = (camRight_.z > 0.0f) ? 1.0f : -1.0f;
                    }
                }


                void PlayerLocomotion::ApplyStickToMoveSpeed(const Vector3& stickInput)
                {
                    pMoveSpeed_->x = (camRight_.x * stickInput.x + camForward_.x * stickInput.z) * *pWalkSpeed_;
                    pMoveSpeed_->z = (camRight_.z * stickInput.x + camForward_.z * stickInput.z) * *pWalkSpeed_;
                }


                void PlayerLocomotion::CalculateMovement(const Vector3& stickInput)
                {
                    if (pCameraManager_ == nullptr)
                        return;

                    UpdateCameraAxes();
                    ApplyStickToMoveSpeed(stickInput);
                }
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp
