#pragma once

namespace nsApp
{
    namespace nsPresentation
    {
        /**
         * @class IBossHudData
         * @brief ボス HUD Screen が参照する HP データのインターフェース。
         *
         * 表示/非表示は Screen 側（BossHudScreen::SetVisible）が担当する。
         * この IF は HP 比率の取得のみを提供する。
         */
        class IBossHudData
        {
        public:
            virtual ~IBossHudData() {}

            /**
             * @brief 現在 HP の割合を返す。
             * @return 0.0f〜1.0f。データ未設定時は 0.0f。
             */
            virtual float GetHpRatio() const = 0;

            /**
             * @brief 最大HPを取得。
             * @return 最大HP。
             */
            virtual float GetMaxHp() const = 0;

            /**
             * @brief 現在HPを取得。
             * @return　現在HP。
             */
            virtual float GetCurrentHp() const = 0;
        };
    } // namespace nsPresentation
} // namespace nsApp
