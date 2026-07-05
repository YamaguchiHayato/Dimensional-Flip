#include "stdafx.h"

#include "BossHubData.h"
#include "Src/Framework/Components/HealthComponent.h"
#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"

namespace nsApp
{
    namespace nsPresentation
    {
        void BossHudData::SetScreen(nsUI::BossHudScreen* pScreen)
        {
            pScreen_ = pScreen;
        }


        void BossHudData::SetHealthComponent(nsFramework::HealthComponent* pHealth)
        {
            pHealth_ = pHealth;

            if (pHealth_ == nullptr)
                return;

            pHealth_->SetOnHpChanged(
                [this](float current, float maxHp)
                {
                    (void) current;
                    (void) maxHp;
                    SyncFromHealthComponent();
                });

            SyncFromHealthComponent();
        }


        float BossHudData::GetCurrentHp() const
        {
            if (pHealth_ == nullptr)
                return 0.0f;
            return pHealth_->GetCurrentHp();
        }


        float BossHudData::GetMaxHp() const
        {
            if (pHealth_ == nullptr)
                return 0.0f;
            return pHealth_->GetMaxHp();
        }


        float BossHudData::GetHpRatio() const
        {
            const float maxHp = GetMaxHp();
            if (maxHp <= 0.0f)
                return 0.0f;
            return GetCurrentHp() / maxHp;
        }


        void BossHudData::SyncFromHealthComponent()
        {
            if (pScreen_ != nullptr)
                pScreen_->SyncFromDataSource();
        }
    } // namespace nsPresentation
} // namespace nsApp
