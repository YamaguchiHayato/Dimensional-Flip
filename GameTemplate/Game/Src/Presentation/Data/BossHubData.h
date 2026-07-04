#pragma once

#include "IBossHubData.h"
/**
 * @file   BossHudData.h
 * @brief  ボス HUD 用 HP データの具体実装。
 * @note   F2 まで BossUIManager へ中継する。F2 で BossHudScreen に差し替える。
 */

namespace nsApp
{
    namespace nsUI
    {
        class BossHudScreen;
    } 

    namespace nsPresentation
    {
        /**
         * @class BossHudData
         * @brief IBossHudData の実装。HP 値を保持し UI へ通知する。
         */
        class BossHudData : public IBossHudData
        {
        public:
            /**
             * @brief HP 値を設定する。
             * @param currentHp 現在のHP。
             * @param maxHp 最大HP。
             */
            void SetHp(float currentHp, float maxHp) override;

            /**
             * @brief HUD スクリーンを設定する。
             * @param pScreen HUD スクリーン。nullptr 可。
             */
            void SetScreen(nsUI::BossHudScreen* pScreen)
            {
                pScreen_ = pScreen;
            }

            /**
             * @brief 現在のHPを取得する。
             * @return 現在のHP。
             */
            float GetCurrentHp() const override { return currentHp_; }

            /**
             * @brief 最大HPを取得する。
             * @return 最大HP。
             */
            float GetMaxHp() const override { return maxHp_; }


        private:
            float currentHp_ = 0.0f;
            float maxHp_ = 0.0f;
            nsUI::BossHudScreen* pScreen_ = nullptr;
        };
    } // namespace nsPresentation
} // namespace nsApp
