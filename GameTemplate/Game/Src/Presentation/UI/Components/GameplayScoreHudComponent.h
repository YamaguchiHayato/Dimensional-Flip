#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"
#include "Src/Presentation/UI/Logic/ScoreDisplayLogic.h"

/**
 * @file   GameplayScoreHudComponent.h
 * @brief  SCORE ラベル + 数値を描画する Gameplay HUD Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class GameplayScoreHudComponent
         * @brief SCORE ラベル + 数値を描画する Component。
         */
        class GameplayScoreHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief 数値の表示位置を設定する。
             * @param x 数値 X。
             * @param y 数値 Y。
             */
            void SetNumberPosition(float x, float y);

            /**
             * @brief Logic の結果を反映する。
             * @param logic スコア表示ロジック。
             */
            void ApplyLogic(const ScoreDisplayLogic& logic);

            /**
             * @brief 初回構築。
             */
            void OnBuild() override;

            /**
             * @brief 描画。
             * @param rc 描画コンテキスト。
             * @param ownerWorld 親 Entity のワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;

            /**
             * @brief 表示/非表示を切り替える。
             * @param visible true で描画する。
             */
            void SetVisible(bool visible) { isVisible_ = visible; }

        private:
            float numPosX_ = 625.0f; //! SCORE 数値の X 座標。
            float numPosY_ = 550.0f; //! SCORE 数値の Y 座標。
            int cachedScore_ = 0;    //! ! SCORE 数値のキャッシュ。
            bool isVisible_ = true;  //! 描画するかどうかのフラグ。
            FontRender scoreFont_;   //! SCORE 数値の描画用フォント。
            FontRender scoreLabelFont_; //! SCORE ラベルの描画用フォント。
            wchar_t scoreText_[32];     //! SCORE 数値の文字列バッファ。
        };
    } // namespace nsUI
} // namespace nsApp
