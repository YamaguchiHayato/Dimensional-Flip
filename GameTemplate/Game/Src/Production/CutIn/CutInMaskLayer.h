#pragma once
#include "Src/Production/CutIn/CutInBase.h"

namespace nsApp
{
    namespace nsProduction
    {
        class CutInMaskLayer : public CutInBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInMaskLayer() = default;
            virtual ~CutInMaskLayer() = default;


        public:
            /**
             * @brief 初期化処理。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。
             */
            void Update() override;

            /**
             * @brief 描画処理。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief レイヤー DDS のパスを組み立てる。
             * @param layerName ファイル名（拡張子なし）。
             * @return Assets 配下のフルパス。
             */
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }
        };
    } // namespace nsProduction
} // namespace nsApp
