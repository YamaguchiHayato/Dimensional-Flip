#pragma once

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PlayerHpBarLogic
         * @brief プレイヤー HP 比率からバー幅を計算する。
         */
        class PlayerHpBarLogic
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            PlayerHpBarLogic();

            /**
             * @brief バー幅を設定する。
             * @param width バー幅。
             */
            void SetBarWidth(float width);

            /**
             * @brief HP 比率を設定する。
             * @param ratio HP 比率。0.0f から 1.0f の範囲で指定する。
             */
            void SetHpRatio(float ratio);

            /**
             * @brief HP 比率を取得する。
             * @return HP 比率。
             */
            float GetHpRatio() const { return hpRatio_; }

            /**
             * @brief バー幅を取得する。
             * @return バー幅。
             */
            float GetBarWidth() const { return barWidth_; }

            /**
             * @brief HP 比率に応じたバーの塗りつぶし幅を取得する。
             * @return バーの塗りつぶし幅。
             */
            float GetFillWidth() const;


        private:
            float barWidth_; //! < バー幅。
            float hpRatio_;  //! < HP 比率。0.0f から 1.0f の範囲で指定する。
        };
    } // namespace nsUI
} // namespace nsApp
