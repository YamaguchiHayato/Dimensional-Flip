#pragma once

namespace nsApp
{
    namespace nsUI
    {
        class TimerDisplayLogic;

        /**
         * @class UITimerView
         * @brief TimerDisplayLogic の結果を描画する View。
         *
         * Phase A: Logic の結果を保持するだけ（Draw は空）。
         * Phase B: UISpriteView で BossHudScreen と同型の描画を追加する。
         *
         * @note .h では TimerDisplayLogic を include しない（前方宣言のみ）。
         *       完全型が必要な処理は .cpp 側で行う。
         */
        class UITimerView
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            UITimerView();

            /**
             * @brief 描画位置を設定する。
             * @param x X座標。
             * @param y Y座標。
             */
            void SetPosition(float x, float y);

            /**
             * @brief 桁間隔を設定する。
             * @param spacing 桁間隔（ピクセル）。
             */
            void SetDigitSpacing(float spacing);

            /**
             * @brief TimerDisplayLogic の結果を適用する。
             * @param logic TimerDisplayLogic の参照。
             */
            void ApplyLogic(const TimerDisplayLogic& logic);

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);


        private:
            float posX_; //! x座標。
            float posY_; //! y座標。
            float digitSpacing_; //! 桁間隔（ピクセル）。
            int cachedSeconds_;  //! TimerDisplayLogic から適用された残り秒数のキャッシュ。
        };
    } /* namespace nsUI */
} /* namespace nsApp */
