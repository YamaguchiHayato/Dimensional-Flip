#pragma once
#include "Src/Scene/title/TitleLayerBase.h"

namespace app
{
    namespace title
    {
        class TitleInformationLayer : public TitleLayerBase
        {
        public:
            TitleInformationLayer() = default;
            virtual ~TitleInformationLayer() = default;


        public:
            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override {};
            // 描画処理。
            void Render(RenderContext& rc) override;


        private:
            // バージョン表記テキストを初期化する。
            void InitVersionText();

            // 権利表記テキストを初期化する。
            void InitCopyRightText();


        // ゲッター。
        public:
            // 画像をセット。
            inline const std::string Init(const std::string layerName) override
            {
                return TitleLayerBase::Init(layerName);
            }

            // 表示フラグをセット。
            inline void SetVisible(bool isVisble)
            {
                isVisible_ = isVisble;
            }


        private:
            // Version表記用(随時更新します)。
            FontRender versionText_;

            // 権利表記用
            FontRender copyRightText_;

            bool isVisible_ = true;

        };
    }
}
