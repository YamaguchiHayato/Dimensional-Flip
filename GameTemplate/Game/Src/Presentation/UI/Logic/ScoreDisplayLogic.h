#pragma once

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class ScoreDisplayLogic
         * @brief スコアを表示用桁配列へ変換する。
         */
        class ScoreDisplayLogic
        {
        public:
            static const int kMaxDigits = 8; //! < 最大桁数。

            /**
             * @brief コンストラクタ。
             */
            ScoreDisplayLogic();    

            /**
             * @brief スコアを設定する。
             * @param score スコア。負数は 0 扱い。
             */
            void SetScore(int score);

            /**
             * @brief 表示桁数を取得する。
             * @return 表示桁数。
             */
            int GetDigitCount() const { return digitCount_; }

            /**
             * @brief 指定桁の値を取得する。
             * @param index 桁インデックス。0〜(GetDigitCount()-1)。
             * @return 指定桁の値。範囲外の場合は 0。
             */
            int GetDigit(int index) const;


        private:
            /**
             * @brie f スコアを桁値に変換する。
             */
            void RebuildDigits();


        private:
            int score_; //! < スコア。
            int digits_[kMaxDigits]; //! < 表示用桁値。
            int digitCount_;         //! < 表示桁数。
        };
    } // namespace nsUI
} // namespace nsApp
