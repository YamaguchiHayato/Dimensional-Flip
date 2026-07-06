#include "stdafx.h"

#include "Src/Presentation/UI/Components/UIScoreView.h"

namespace nsApp
{
    namespace nsUI
    {
        UIScoreView::UIScoreView() : posX_(0.0f), posY_(0.0f), digitSpacing_(20.0f) {}

        void UIScoreView::SetPosition(float x, float y)
        {
            posX_ = x;
            posY_ = y;
        }

        void UIScoreView::SetDigitSpacing(float spacing)
        {
            digitSpacing_ = spacing;
        }

        void UIScoreView::ApplyLogic(const ScoreDisplayLogic& logic)
        {
            cachedLogic_ = logic;
        }

        void UIScoreView::Draw(RenderContext& rc)
        {
            (void) rc;
        }
    } // namespace nsUI
} // namespace nsApp
