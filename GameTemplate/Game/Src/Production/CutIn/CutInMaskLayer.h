#pragma once
#include "Src/production/CutIn/CutInBase.h"
namespace app
{
    namespace cutIn
    {
        class CutInMaskLayer : public CutInBase
        {
        public:
            CutInMaskLayer() = default;
            virtual ~CutInMaskLayer() = default;

            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override;
            // 描画処理。
            void Render(RenderContext& rc) override;
            // レイヤーのパスを取得する。
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }


        };

    }
}
