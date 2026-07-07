#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"
#include "Src/Presentation/UI/Logic/TImeDisplayLogic.h"

/**
 * @file   GameplayTimerHudComponent.h
 * @brief  タイマー背景 + 数字を描画する Gameplay HUD Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class GameplayTimerHudComponent
         * @brief タイマー背景 + 数字を描画する Gameplay HUD Component。
         */
        class GameplayTimerHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief 数字の表示位置を設定する。
             * @param x 数字 X。
             * @param y 数字 Y。
             */
            void SetTextPosition(float x, float y);

            /**
             * @brief Logic の結果を反映する。
             * @param logic タイマー表示ロジック。
             */
            void ApplyLogic(const TimerDisplayLogic& logic);

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


        private:
            float textPosX_ = -940.0f; //! タイマー数字の X 座標。
            float textPosY_ = 430.0f;  //! タイマー数字の Y 座標。
            int cachedSeconds_ = 0;    //! < キャッシュされた秒数。

            SpriteRender timerBar_; //! < タイマー背景スプライト。
            FontRender timerFont_;  //! < タイマー数字フォント。
            wchar_t timerText_[8];  //! < タイマー数字テキストバッファ。
            bool timerBarInitialized_ = false; //! < タイマー背景スプライトが初期化済みかどうか。
        };
    } // namespace nsUI
} // namespace nsApp
