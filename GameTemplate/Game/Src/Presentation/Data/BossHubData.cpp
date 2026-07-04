#include "stdafx.h"
#include "BossHubData.h"
#include "Src/Core/BossUIManager.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"

namespace nsApp
{
    namespace nsPresentation
    {
        void BossHudData::SetHp(float currentHp, float maxHp)
        {
            currentHp_ = currentHp;
            maxHp_ = maxHp;

            /* 新しいUIへ反映する。*/
            if (pScreen_ != nullptr)
                pScreen_->SyncFromDataSource();
        }
    } // namespace nsPresentation
} // namespace nsApp
