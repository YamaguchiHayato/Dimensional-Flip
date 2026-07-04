#include "stdafx.h"

#include "HealthComponent.h"
#include "Src/Presentation/Data/IBossHubData.h"

namespace nsApp
{
    namespace nsFramework
    {
        void HealthComponent::Initialize(float maxHp, nsPresentation::IBossHudData* pHudData)
        {
            /* 初期化。*/
            maxHp_ = maxHp;
            currentHp_ = maxHp;
            pHudData_ = pHudData;

            /* 初期 HP を UI へ反映する */
            NotifyHud();
        }


        void HealthComponent::SetCurrentHp(float hp)
        {
            /* 現在のHPのデータをセットする。*/
            currentHp_ = hp;

            /* 0 未満にはしない */
            if (currentHp_ < 0.0f)
                currentHp_ = 0.0f;

            /* 最大 HP を超えない */
            if (currentHp_ > maxHp_)
                currentHp_ = maxHp_;

            
            NotifyHud();
        }


        void HealthComponent::ApplyDamage(float damage)
        {
            /* ダメージ量が 0 以下なら何もしない */
            if (damage <= 0.0f)
                return;

            SetCurrentHp(currentHp_ - damage);
        }


        void HealthComponent::NotifyHud() const
        {
            if (pHudData_ != nullptr)
                pHudData_->SetHp(currentHp_, maxHp_);
        }
    } // namespace nsFramework
} // namespace nsApp
