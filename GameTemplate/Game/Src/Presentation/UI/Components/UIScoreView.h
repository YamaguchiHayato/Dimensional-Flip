#pragma once

#include "Src/Presentation/UI/Logic/ScoreDisplayLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIScoreView
         * @brief スコアを Font で描画する View（旧 ScoreUI 互換）。
         */
        class UIScoreView
        {
        public:
            /**
             * @brief UIScoreView のコンストラクタ。
             */
            UIScoreView();

            /**
             * @brief UIScoreView のデストラクタ。
             * @param x X 座標。
             * @param y Y 座標。
             */
            void SetPosition(float x, float y);

            /**
             * @brief UIScoreView のデストラクタ。
             * @param spacing 桁間隔。
             */
            void SetDigitSpacing(float spacing);

            /**
             * @brief スコアロジックを適応する。
             * @param logic スコア表示ロジック。
             */
            void ApplyLogic(const ScoreDisplayLogic& logic);

            /**
             * @brief UIScoreView の描画。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief スコア表示の表示/非表示を切り替える。
             * @param visible true で表示。
             */
            void SetVisible(bool visible)
            {
                isVisible_ = visible;
            }


        private:
            /**
             * @brief UIScoreView のフォントを初期化する。
             */
            void InitFonts();


        private:
            float posX_; //! < X 座標。
            float posY_; //! < Y 座標。
            float digitSpacing_; //! < 桁間隔。
            bool isVisible_ = true;

            FontRender scoreFont_;      // 数値（000000）
            FontRender scoreLabelFont_; // "SCORE" ラベル
            wchar_t scoreText_[32];

            int cachedScore_;
        };
    } // namespace nsUI
} // namespace nsApp
