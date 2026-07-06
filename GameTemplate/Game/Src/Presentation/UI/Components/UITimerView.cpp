#include "stdafx.h"

#include "GameplayHudUiNames.h"
#include "UITimerView.h"

namespace nsApp
{
    namespace nsUI
    {
        UITimerView::UITimerView() : posX_(0.0f), posY_(0.0f), digitSpacing_(24.0f), isColonVisible_(false)
        {
            colonView_.SetUiName(FetchUIName(GameplayHudUiId::TimerColon));
        }

        void UITimerView::SetPosition(float x, float y)
        {
            posX_ = x;
            posY_ = y;
        }

        void UITimerView::SetDigitSpacing(float spacing)
        {
            digitSpacing_ = spacing;
        }

        void UITimerView::ApplyLogic(const TimerDisplayLogic& logic)
        {
            float offsetX = 0.0f;
            isColonVisible_ = false;

            for (int i = 0; i < TimerDisplayLogic::kMaxDigits; ++i)
            {
                const int digit = logic.GetDigit(i);
                digitViews_[i].SetUiName(FetchUIName(GetTimerDigitId(digit)));
                digitViews_[i].SetPosition(posX_ + offsetX, posY_);
                offsetX += digitSpacing_;

                if (logic.NeedsColonAfter(i))
                {
                    colonView_.SetPosition(posX_ + offsetX, posY_);
                    isColonVisible_ = true;
                    offsetX += digitSpacing_;
                }
            }
        }

        void UITimerView::Draw(RenderContext& rc)
        {
            for (int i = 0; i < TimerDisplayLogic::kMaxDigits; ++i)
                digitViews_[i].Draw(rc);

            if (isColonVisible_)
                colonView_.Draw(rc);
        }

        GameplayHudUiId UITimerView::GetTimerDigitId(int digit) const
        {
            switch (digit)
            {
            case 0:
                return GameplayHudUiId::TimerDigit0;
            case 1:
                return GameplayHudUiId::TimerDigit1;
            case 2:
                return GameplayHudUiId::TimerDigit2;
            case 3:
                return GameplayHudUiId::TimerDigit3;
            case 4:
                return GameplayHudUiId::TimerDigit4;
            case 5:
                return GameplayHudUiId::TimerDigit5;
            case 6:
                return GameplayHudUiId::TimerDigit6;
            case 7:
                return GameplayHudUiId::TimerDigit7;
            case 8:
                return GameplayHudUiId::TimerDigit8;
            case 9:
                return GameplayHudUiId::TimerDigit9;
            default:
                return GameplayHudUiId::TimerDigit0;
            }
        }
    } // namespace nsUI
} // namespace nsApp
