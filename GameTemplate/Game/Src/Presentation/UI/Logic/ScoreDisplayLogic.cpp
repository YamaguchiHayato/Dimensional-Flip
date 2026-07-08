#include "stdafx.h"

#include "Src/Presentation/UI/Logic/ScoreDisplayLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        ScoreDisplayLogic::ScoreDisplayLogic() : score_(0), digitCount_(1)
        {
            for (int i = 0; i < kMaxDigits; ++i)
                digits_[i] = 0;
        }


        void ScoreDisplayLogic::SetScore(int score)
        {
            score_ = (score < 0) ? 0 : score;
            RebuildDigits();
        }


        int ScoreDisplayLogic::GetDigit(int index) const
        {
            if (index < 0 || index >= digitCount_)
                return 0;
            return digits_[index];
        }


        void ScoreDisplayLogic::RebuildDigits()
        {
            int value = score_;
            digitCount_ = 0;

            if (value == 0)
            {
                digits_[0] = 0;
                digitCount_ = 1;
                return;
            }

            /* 下位桁から取り出す */
            while (value > 0 && digitCount_ < kMaxDigits)
            {
                digits_[digitCount_] = value % 10;
                value /= 10;
                ++digitCount_;
            }

            /* 表示順（上位桁が先）に反転 */
            for (int i = 0; i < digitCount_ / 2; ++i)
            {
                const int tmp = digits_[i];
                digits_[i] = digits_[digitCount_ - 1 - i];
                digits_[digitCount_ - 1 - i] = tmp;
            }
        }
    } // namespace nsUI
} // namespace nsApp
