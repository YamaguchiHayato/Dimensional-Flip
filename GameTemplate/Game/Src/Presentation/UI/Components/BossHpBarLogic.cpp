#include "stdafx.h"

#include "Src/Presentation/UI/Components/BossHpBarLogic.h"
#include "Src/Presentation/UI/Components/UISpriteView.h"

namespace
{
    const float DAMAGE_BAR_LERP_SPEED = 0.005f;
}

namespace nsApp
{
    namespace nsUI
    {
        void BossHpBarLogic::Setup(UISpriteView* pCurrentBar, UISpriteView* pDamageBar)
        {
            pCurrentBar_ = pCurrentBar;
            pDamageBar_ = pDamageBar;
        }


        void BossHpBarLogic::SetHpPercent(float percent)
        {
            /* 0～1 にクランプする */
            if (percent < 0.0f)
                percent = 0.0f;
            if (percent > 1.0f)
                percent = 1.0f;

            currentPercent_ = percent;
        }


        void BossHpBarLogic::OnUpdate(float deltaTime)
        {
            (void) deltaTime;

            /* ダメージバーを current へゆっくり追従させる */
            if (damagePercent_ > currentPercent_)
            {
                damagePercent_ -= DAMAGE_BAR_LERP_SPEED;
                if (damagePercent_ < currentPercent_)
                    damagePercent_ = currentPercent_;
            }
            else
            {
                damagePercent_ = currentPercent_;
            }

            /* スプライトの X スケールへ反映する */
            if (pCurrentBar_ != nullptr)
                pCurrentBar_->SetScale(Vector3(currentPercent_, 1.0f, 1.0f));

            if (pDamageBar_ != nullptr)
                pDamageBar_->SetScale(Vector3(damagePercent_, 1.0f, 1.0f));
        }
    } // namespace nsUI
} // namespace nsApp
