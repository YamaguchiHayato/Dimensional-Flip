#include "stdafx.h"

#include "Src/Production/CutIn/CutInSlideLayer.h"

namespace
{
    struct layerSize
    {
        static constexpr auto WIDHT = 220.0;
        static constexpr auto HEIGHT = 185.0f;
    };
} // namespace

namespace nsApp
{
    namespace nsProduction
    {
        bool CutInSlideLayer::Start()
        {
            const std::string layerPath_Normal = FindLayerPath("BOSS");
            layer_.Init(layerPath_Normal.c_str(), layerSize::WIDHT, layerSize::HEIGHT);

            InitStartPos();
            InitSlideParam();
            InitWaitTimer();
            return true;
        }

        void CutInSlideLayer::Update()
        {
            if (waitTimer_ < waitTime_)
            {
                waitTimer_ += 1.0f / 60.0f;
                layer_.SetPosition(startPos_);
                layer_.Update();
                return;
            }

            Slide();

            if (!isMoving_)
            {
            }
            else
            {
                float scale = 3.0f;
                layer_.SetScale(Vector3(scale, scale, scale));
            }

            layer_.SetPosition(currentPos_);
            layer_.Update();
        }

        void CutInSlideLayer::Slide()
        {
            if (isMoving_)
            {
                float deltaTime = 1.0f / 60.0f;
                timer_ += deltaTime;

                float rate = timer_ / duration_;
                if (rate >= 1.0f)
                {
                    rate = 1.0f;
                    isMoving_ = false;
                }

                float t = 1.0f - rate;
                float easeRate = 1.0f - (t * t * t);
                currentPos_ = startPos_ + (endPos_ - startPos_) * easeRate;
            }
        }

        void CutInSlideLayer::Breathing()
        {
            breathTimer_ += 1.0f / 60.0f;
            float scale = 1.0f + sinf(breathTimer_ * 5.0f) * 0.05f;
            layer_.SetScale(Vector3(scale, scale, scale));
        }

        void CutInSlideLayer::Render(RenderContext& rc)
        {
            layer_.Draw(rc);
        }
    } // namespace nsProduction
} // namespace nsApp
