#include "stdafx.h"

#include "Src/Presentation/UI/Components/UITimerView.h"
#include "Src/Presentation/UI/Logic/TImeDisplayLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        UITimerView::UITimerView() : posX_(0.0f), posY_(0.0f), digitSpacing_(24.0f), cachedSeconds_(0) {}

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
            cachedSeconds_ = logic.GetSeconds();

            /* Phase B: BossHudScreen と同じ UISpriteView API で桁スプライトを配置 */
            for (int i = 0; i < TimerDisplayLogic::kMaxDigits; ++i)
            {
                const int digit = logic.GetDigit(i);
                (void) digit;

                if (logic.NeedsColonAfter(i))
                {
                    /* コロン位置の計算は Phase B で実装 */
                }
            }

            (void) posX_;
            (void) posY_;
            (void) digitSpacing_;
        }

        void UITimerView::Draw(RenderContext& rc)
        {
            (void) rc;
            (void) cachedSeconds_;

            /* Phase B: digitViews_ / colonView_ を Draw */
        }
    } /* namespace nsUI */
} /* namespace nsApp */
