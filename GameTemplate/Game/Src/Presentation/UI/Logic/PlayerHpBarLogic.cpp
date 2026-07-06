#include "stdafx.h"

#include <algorithm>

#include "PlayerHpBarLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        PlayerHpBarLogic::PlayerHpBarLogic() : barWidth_(200.0f), hpRatio_(1.0f) {}


        void PlayerHpBarLogic::SetBarWidth(float width)
        {
            barWidth_ = (width < 0.0f) ? 0.0f : width;
        }


        void PlayerHpBarLogic::SetHpRatio(float ratio)
        {
            hpRatio_ = (std::max)(0.0f, (std::min)(ratio, 1.0f));
        }


        float PlayerHpBarLogic::GetFillWidth() const
        {
            return barWidth_ * hpRatio_;
        }
    } // namespace nsUI
} // namespace nsApp
