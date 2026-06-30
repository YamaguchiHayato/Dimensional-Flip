#pragma once
#include "Src/Scene/title/TitleLayerBase.h"

namespace nsApp
{
    namespace nsTitle
    {
        /**
         * @class TitleLogoLayer
         * @brief タイトルロゴの表示レイヤー。
         */
        class TitleLogoLayer : public TitleLayerBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleLogoLayer() = default;
            virtual ~TitleLogoLayer() = default;


        public:
            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 毎フレームのシーン更新（遷移判定・入力など）。
             */
            void Update() override {};

            /**
             * @brief 毎フレームのシーン描画。
             * @param rc 
             */
            void Render(RenderContext& rc) override;


        private:
            /**
             * @brief タイトルロゴのテキストを初期化する。
             */
            void InitTitleLogoText();


        public:
            /**
             * @brief タイトルロゴの表示・非表示を設定する。
             */
            inline void SetVisible(bool isVisble) { isVisible_ = isVisble; }


        public:
            /**
             * @brief レイヤーの初期化。レイヤー名を設定する。
             * @param layerName レイヤーネーム。
             * @return ファイルパス。
             */
            inline const std::string Init(const std::string layerName) override
            {
                return TitleLayerBase::Init(layerName);
            }


        private:
            FontRender titleFont_;

            bool isVisible_ = true;
        };
    } // namespace nsScene
} // namespace nsApp
