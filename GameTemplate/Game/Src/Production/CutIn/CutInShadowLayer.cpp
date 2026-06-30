#include "stdafx.h"

#include "Src/Production/CutIn/CutInShadowLayer.h"
#include "Src/Production/CutIn/CutInSlideLayer.h"

namespace
{
    struct layerSize
    {
        static constexpr float WIDTH = 225.0f;
        static constexpr float HEIGHT = 185.0f;
    };
} // namespace

namespace nsApp
{
    namespace nsProduction
    {
        bool CutInShadowLayer::Start()
        {
            const std::string layerName = FindLayerPath("Black");
            layer_.Init(layerName.c_str(), layerSize::WIDTH, layerSize::HEIGHT);

            pTargetLayer_ = nullptr;
            pTargetLayer_ = FindGO<CutInSlideLayer>("SlideLayer");

            if (offset_.Length() == 0.0)
                offset_ = Vector3(0.0f, 0.0f, 0.0f);

            layer_.SetAlpha(0.0f);
            return true;
        }


        void CutInShadowLayer::Update()
        {
            if (pTargetLayer_ == nullptr)
                return;

            if (!pTargetLayer_->IsMoving())
            {
                currentPos_ = pTargetLayer_->GetCurrentPos();
                ejectStartPos_ = currentPos_;
                layer_.SetAlpha(0.0f);
                ejectTimer_ = 0.0f;
            }
            else
            {
                if (ejectTimer_ < 1.0f)
                {
                    layer_.SetAlpha(0.5f);

                    float ejectDuration = 0.2f;
                    ejectTimer_ += (1.0f / 60.0f) / ejectDuration;

                    if (ejectTimer_ > 1.0f)
                        ejectTimer_ = 1.0f;

                    float t = 1.0f - ejectTimer_;
                    float ease = 1.0f - (t * t * t);

                    Vector3 targetPos = ejectStartPos_ + offset_;
                    currentPos_ = targetPos + (targetPos - ejectStartPos_) * ease;
                }
                else
                {
                    Vector3 targetPos = pTargetLayer_->GetCurrentPos();
                    currentPos_ = targetPos + offset_;
                }
            }

            layer_.SetPosition(currentPos_);
            float scale = 1.0f;
            layer_.SetScale(Vector3(scale, scale, scale));
            layer_.Update();
        }


        void CutInShadowLayer::Render(RenderContext& rc)
        {
            layer_.Draw(rc);
        }
    } // namespace nsProduction
} // namespace nsApp
