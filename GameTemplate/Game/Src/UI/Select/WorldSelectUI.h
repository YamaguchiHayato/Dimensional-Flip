#pragma once

/**
 * @file   WorldSelectUI.h
 * @brief  ワールド選択画面の 2D UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class WorldSelectUI
         * @brief ステージ名・操作ガイド・Back/Select 表記を描画する。
         */
        class WorldSelectUI : public IGameObject
        {
        public:
            WorldSelectUI() = default;
            virtual ~WorldSelectUI() = default;

            /** @brief フォント・スプライトを初期化する。 */
            void Init();

            void Update() override;

            /**
             * @brief UI を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief 選択中ステージ名を更新する。
             * @param name 表示するワイド文字列。
             */
            void SetStageName(const wchar_t* name);


        private:
            /**
             * @brief UI の描画処理。Render() から呼ばれる。
             */
            void DrawStageName();
            void DrawStageSelectText();
            void DrawStickUI();
            void DrawSelectText();
            void DrawBackButtonUI();
            void DrawBackText();
            void DrawSkipText();
            void TrySkip();


        private:
            FontRender   stageNameText_;      //!< 左上ステージ名
            FontRender   worldSelectTextUI_;  //!< 上部タイトル
            SpriteRender stickUIRender_;      //!< 左下スティック UI
            FontRender   selectTextUI_;       //!< Select 表記
            SpriteRender backButtonRender_;   //!< 右下 B ボタン
            FontRender   backUIText_;         //!< Back 表記
        };
    } // namespace nsUI
} // namespace nsApp

using WorldSelectUI = nsApp::nsUI::WorldSelectUI;
