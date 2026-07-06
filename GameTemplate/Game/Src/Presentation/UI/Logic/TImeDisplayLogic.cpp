#include "stdafx.h"

#include "TImeDisplayLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        TimerDisplayLogic::TimerDisplayLogic() : seconds_(0)
        {
            for (int i = 0; i < kMaxDigits; ++i)
                digits_[i] = 0;
        }

        void TimerDisplayLogic::SetSeconds(int seconds)
        {
            seconds_ = (seconds < 0) ? 0 : seconds;
            RebuildDigits();
        }

        int TimerDisplayLogic::GetSeconds() const
        {
            return seconds_;
        }

        int TimerDisplayLogic::GetDigit(int index) const
        {
            if (index < 0 || index >= kMaxDigits)
                return 0;
            return digits_[index];
        }

        bool TimerDisplayLogic::NeedsColonAfter(int index) const
        {
            /* 分と秒の間（index 1 の後）にコロン */
            return (index == 1);
        }

        void TimerDisplayLogic::RebuildDigits()
        {
            const int min = seconds_ / 60;
            const int sec = seconds_ % 60;

            digits_[0] = (min / 10) % 10;
            digits_[1] = min % 10;
            digits_[2] = (sec / 10) % 10;
            digits_[3] = sec % 10;
        }
    } /* namespace nsUI */
} /* namespace nsApp */
