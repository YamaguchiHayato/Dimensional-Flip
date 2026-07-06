#include "stdafx.h"

#include "Src/Presentation/UI/Components/UIPlayerHpBarView.h"

namespace nsApp
{
    namespace nsUI
    {
        UIPlayerHpBarView::UIPlayerHpBarView() : posX_(0.0f), posY_(0.0f) {}


        void UIPlayerHpBarView::SetPosition(float x, float y)
        {
            posX_ = x;
            posY_ = y;
        }

        void UIPlayerHpBarView::ApplyLogic(const PlayerHpBarLogic& logic)
        {
            cachedLogic_ = logic;

            /* Phase B: backView_ / fillView_ を BossHudScreen 同様に更新 */
        }

        void UIPlayerHpBarView::Draw(RenderContext& rc)
        {
            (void) rc;

            /* Phase B: UISpriteView::Draw を呼ぶ */
            (void) cachedLogic_;
        }
    } // namespace nsUI
} // namespace nsApp
