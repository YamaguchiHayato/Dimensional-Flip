#pragma once

#include "IBossHubData.h"
namespace nsApp
{
    namespace nsFramework
    {
        class HealthComponent;
    }
    namespace nsUI
    {
        class BossHudScreen;
    }

    namespace nsPresentation
    {
        class BossHudData : public IBossHudData
        {
        public:
            /**
             * @brief UIをセット。
             * @param pScreen BossのUI。
             */
            void SetScreen(nsUI::BossHudScreen* pScreen);

            /**
             * @brief 
             * @param pHealth 
             */
            void SetHealthComponent(nsFramework::HealthComponent* pHealth);

            /**
             * @brief 現在のHPを取得。
             * @return 
             */
            float GetCurrentHp() const override;

            /**
             * @brief 最大HPを取得。
             * @return 
             */
            float GetMaxHp() const override;

            /**
             * @brief HPの割合を取得。
             * @return 
             */
            float GetHpRatio() const override;

            /**
             * @brief 
             */
            void SyncFromHealthComponent();


        private:
            nsUI::BossHudScreen* pScreen_ = nullptr;
            nsFramework::HealthComponent* pHealth_ = nullptr;
        };
    } // namespace nsPresentation
} // namespace nsApp
