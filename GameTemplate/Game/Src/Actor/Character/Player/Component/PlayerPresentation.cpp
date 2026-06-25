#include "stdafx.h"

#include "PlayerPresentation.h"
#include "Src/Core/CameraManager.h"

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                void PlayerPresentation::Bind(Character2DRender*& pRender, uint8_t& currentIndex, Quaternion& rot, Quaternion& offsetRot, const Vector3& modelScale)
                {
                    ppRender_ = &pRender;
                    pCurrentIndex_ = &currentIndex;
                    pRot_ = &rot;
                    pOffsetRot_ = &offsetRot;
                    modelScale_ = modelScale;
                }


                void PlayerPresentation::SetCurrentIndex(uint8_t index)
                {
                    if (pCurrentIndex_ != nullptr)
                        *pCurrentIndex_ = index;
                }


                void PlayerPresentation::SyncView(const Vector3& pos)
                {
                    if (ppRender_ == nullptr || *ppRender_ == nullptr)
                        return;

                    // モデルの移動。
                    (*ppRender_)->SetPosition(pos);
                    (*ppRender_)->Update();
                }


                void PlayerPresentation::PostFrameUpdate()
                {
                    if (ppRender_ == nullptr || *ppRender_ == nullptr || pCurrentIndex_ == nullptr)
                        return;

                    (*ppRender_)->SetCurrentIndex(*pCurrentIndex_);
                    (*ppRender_)->SetScale(modelScale_);
                    (*ppRender_)->Update();
                }


                void PlayerPresentation::UpdateOffsetRotation(CameraManager* pCameraManager)
                {
                    if (pCameraManager == nullptr || pOffsetRot_ == nullptr)
                        return;

                    const auto cameraType = pCameraManager->GetCurrentCameraMode();

                    if (cameraType == CameraMode::mode2D)
                        pOffsetRot_->SetRotationY(0.0f);
                    else
                        pOffsetRot_->SetRotationY(-90.0f);
                }


                void PlayerPresentation::ApplyRotation()
                {
                    if (ppRender_ == nullptr || *ppRender_ == nullptr || pRot_ == nullptr || pOffsetRot_ == nullptr)
                        return;

                    // プレイヤーの回転をセット。
                    *pRot_ = *pOffsetRot_;
                    (*ppRender_)->SetRotation(*pRot_);
                }


                void PlayerPresentation::Render(RenderContext& rc)
                {
                    if (ppRender_ == nullptr || *ppRender_ == nullptr)
                        return;

                    // キャラモデル。
                    (*ppRender_)->Render(rc);
                }


                Character2DRender* PlayerPresentation::GetRender()
                {
                    if (ppRender_ == nullptr)
                        return nullptr;

                    return *ppRender_;
                }
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor
} // namespace nsApp
