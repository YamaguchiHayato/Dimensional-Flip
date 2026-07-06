#pragma once

#include "Src/Presentation/UI/Logic/ScoreDisplayLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        class UIScoreView
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            UIScoreView();

            /**
             * @brief スコア表示の位置を設定する。
             * @param x X座標。
             * @param y Y座標。
             */
            void SetPosition(float x, float y);

            /**
             * @brief スコア表示の桁間隔を設定する。
             * @param spacing 桁間隔。
             */
            void SetDigitSpacing(float spacing);

            /**
             * @brief スコア表示のロジックを適用する。
             * @param logic スコア表示のロジック。
             */
            void ApplyLogic(const ScoreDisplayLogic& logic);

            /**
             * @brief スコア表示を描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);


        private:
            float posX_; //! スコア表示のX座標。
            float posY_; //! スコア表示のY座標。
            float digitSpacing_; //! スコア表示の桁間隔。
            ScoreDisplayLogic cachedLogic_; //! キャッシュされたスコア表示のロジック。
        };
    } // namespace nsUI
} // namespace nsApp
