#include "stdafx.h"

#include "Src/Production/CutIn/CutInMaskLayer.h"

namespace
{
    struct layerStatus
    {
        static constexpr auto WIDHT = 2000.0f;
        static constexpr auto HEIGHT = 2000.0f;
    };
} // namespace

namespace nsApp
{
    namespace nsProduction
    {
        bool CutInMaskLayer::Start()
        {
            std::string maskLayerPath = FindLayerPath("BagGround");
            layer_.Init(maskLayerPath.c_str(), layerStatus::WIDHT, layerStatus::HEIGHT);
            return true;
        }

        void CutInMaskLayer::Update()
        {
            layer_.SetScale(Vector3::One);
            layer_.SetPosition(Vector3::Zero);
            layer_.Update();
        }


        void CutInMaskLayer::Render(RenderContext& rc)
        {
            layer_.Draw(rc);
        }
    } // namespace nsProduction
} // namespace nsApp
