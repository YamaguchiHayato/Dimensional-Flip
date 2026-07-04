#include "stdafx.h"

#include "BossHubData.h"
#include "Src/Core/BossUIManager.h"

namespace nsApp
{
    namespace nsPresentation
    {
        void BossHudData::SetHp(float currentHp, float maxHp)
        {
            currentHp_ = currentHp;
            maxHp_ = maxHp;

            /*
             * F2 までの暫定: 既存 BossUIManager へ中継
             * F2 で BossHudScreen::Bind(this) に置き換えてこの行を削除
             */
            app::nsUI::BossUIManager::GetInstance().OnUpdateHP(currentHp_, maxHp_);
        }
    } // namespace nsPresentation
} // namespace nsApp
