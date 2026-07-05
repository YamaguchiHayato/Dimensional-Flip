#include "stdafx.h"

#include <algorithm>

#include "HealthComponent.h"

namespace nsApp
{
    namespace nsFramework
    {
        void HealthComponent::Initialize(float maxHp, float currentHp)
        {
            maxHp_ = maxHp;
            currentHp_ = (currentHp < 0.0f) ? maxHp : currentHp;
            NotifyHpChanged();
        }


        void HealthComponent::SetCurrentHp(float hp)
        {
            /* C++14: std::clamp の代わりに min/max */
            currentHp_ = (std::max)(0.0f, (std::min)(hp, maxHp_));
            NotifyHpChanged();
        }


        void HealthComponent::TakeDamage(float amount)
        {
            if (amount <= 0.0f)
                return;

            currentHp_ = (std::max)(0.0f, currentHp_ - amount);
            NotifyHpChanged();
        }


        void HealthComponent::SetOnHpChanged(HpChangedCallback callback)
        {
            onHpChanged_ = std::move(callback);
        }


        void HealthComponent::NotifyHpChanged()
        {
            if (onHpChanged_)
                onHpChanged_(currentHp_, maxHp_);
        }
    } // namespace nsFramework
} // namespace nsApp
