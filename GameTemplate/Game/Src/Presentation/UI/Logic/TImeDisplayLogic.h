#pragma once

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class TimerDisplayLogic
         * @brief 残り秒数を MM:SS 表示用の桁値へ変換する。
         */
        class TimerDisplayLogic
        {
        public:
            static const int kMaxDigits = 4; /* MM:SS の 4 桁 */

            TimerDisplayLogic();

            /**
             * @brief 残り秒数を設定する。
             * @param seconds 残り秒数。負数は 0 扱い。
             */
            void SetSeconds(int seconds);

            /**
             * @brief 設定済みの残り秒数を返す。
             */
            int GetSeconds() const;

            /**
             * @brief 表示桁の値を取得する。
             * @param index 0〜3。
             */
            int GetDigit(int index) const;

            /**
             * @brief 指定桁の直後にコロンを挟むか。
             * @param index 桁インデックス。
             */
            bool NeedsColonAfter(int index) const;


        private:
            /**
             * @brief 残り秒数を桁値へ変換する。
             */
            void RebuildDigits();


        private:
            int seconds_; //! < 残り秒数。
            int digits_[kMaxDigits]; //! < MM:SS の各桁値。
        };
    } /* namespace nsUI */
} /* namespace nsApp */
