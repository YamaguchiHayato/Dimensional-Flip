#include "stdafx.h"

// カットインレイヤークラス。
#include "Src/Direction/CutIn/CutInBase.h"
#include "Src/Direction/CutIn/CutInMaskLayer.h"

struct layerStatus
{
    static constexpr auto WIDHT = 2000.0f;  // カットイン黒背景の横幅。
    static constexpr auto HEIGHT = 2000.0f; // カットイン黒背景の縦幅。
};

namespace app
{
    namespace cutIn
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
    }
}
