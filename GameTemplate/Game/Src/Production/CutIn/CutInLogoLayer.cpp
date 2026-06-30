#include "stdafx.h"

#include "Src/Production/CutIn/CutInLogoLayer.h"

namespace layerParam
{
    const auto WIDHT = 1280.0f;
    const auto HEIGHT = 648.0f;
    const auto START_ROT = 720.0f;
    const auto END_ROT = 0.0f;
    Vector3 CURRENT_ROT(300.0f, 0.0f, 0.0f);
    Vector3 POSITION(600.0f, 0.0f, 0.0f);
    const float STARTSCALE = 6.0f;
    const float ENDSCALE = 2.0f;
} // namespace layerParam

namespace nsApp
{
    namespace nsProduction
    {
        bool CutInLogoLayer::Start()
        {
            const std::string layerPath = FindLayerPath("Emblem");
            layer_.Init(layerPath.c_str(), layerParam::WIDHT, layerParam::HEIGHT);
            layer_.SetPosition(Vector3(200.0f, 100.0f, 0.0f));
            Preparation();
            return true;
        }

        void CutInLogoLayer::Update()
        {
            Move();
            layer_.SetRotation(currentRot_);
            layer_.SetPosition(layerParam::POSITION);
            layer_.Update();
        }

        void CutInLogoLayer::Move()
        {
            if (!isVisible)
            {
                float deltaTime = 1.0f / 60.0f;
                delayTimer_ += deltaTime;

                if (delayTimer_ >= delayTime_)
                    CanDrawEmblem();
            }

            if (isVisible && isAnimating_)
            {
                float deltaTime = 1.0f / 60.0f;
                moveTimer_ += deltaTime;

                float rate = moveTimer_ / duration_;
                if (rate >= 1.0f)
                {
                    rate = 1.0f;
                    isAnimating_ = false;
                }

                float t = 1.0f - rate;
                float easeRate = 1.0f - (t * t * t);

                float zAngleDeg = layerParam::START_ROT + (layerParam::END_ROT - layerParam::START_ROT) * easeRate;
                float zAngleRad = zAngleDeg * (3.14159265f / 180.0f);
                currentRot_.SetRotationZ(zAngleRad);

                float currentScale =
                    layerParam::STARTSCALE + (layerParam::ENDSCALE - layerParam::STARTSCALE) * easeRate;
                layer_.SetScale(Vector3(currentScale, currentScale, currentScale));
            }
            else if (isVisible && !isAnimating_)
            {
                auto endScale = 1.0;
                layer_.SetScale(Vector3(endScale, endScale, endScale));
            }
        }

        void CutInLogoLayer::Preparation()
        {
            delayTimer_ = 0.0f;
            delayTime_ = 0.0f;
            moveTimer_ = 0.0f;
            delayTimer_ = 0.6f;
            isVisible = false;
            isAnimating_ = false;
        }

        void CutInLogoLayer::Render(RenderContext& rc)
        {
            if (isVisible)
                layer_.Draw(rc);
        }
    } // namespace nsProduction
} // namespace nsApp
