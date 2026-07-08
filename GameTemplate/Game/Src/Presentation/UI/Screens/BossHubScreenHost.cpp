#include "stdafx.h"

#include "BossHubScreenHost.h"
#include "Src/Core/Game.h"
#include "Src/Core/StageManager.h"
#include "Src/Presentation/Data/BossHubData.h"

namespace nsApp
{
    namespace nsUI
    {
        bool BossHudScreenHost::Start()
        {
            if (!UIScreenHost::Start())
                return false;

            auto* pUiScreen = GetScreen();
            auto* pGame = FindGO<nsCore::Game>("game");
            if (pUiScreen == nullptr || pGame == nullptr)
                return true;

            auto* pScreen = static_cast<BossHudScreen*>(pUiScreen);

            if (auto* pData = pGame->GetBossHudData())
            {
                pData->SetScreen(pScreen);
                pScreen->Bind(pData);
            }

            /* 非ボスステージでは非表示。ボスは OnEnter / Reconnect で true にする */
            bool isBossStage = false;
            if (auto* pStage = nsStage::StageManager::GetInstance())
                isBossStage = (pStage->GetCurrentStageID() == nsStage::StageID::sStageEX);
            if (!isBossStage)
                isBossStage = (nsStage::StageManager::GetNextInitStageID() == nsStage::StageID::sStageEX);

            if (!isBossStage)
                pScreen->SetVisible(false);
            /* true のときは触らない（デフォルト true のまま） */

            return true;
        }


        std::unique_ptr<UIScreen> BossHudScreenHost::CreateScreen()
        {
            return std::make_unique<BossHudScreen>();
        }


        BossHudScreen* BossHudScreenHost::GetBossHudScreen()
        {
            if (screen_ == nullptr)
                return nullptr;

            return static_cast<BossHudScreen*>(screen_.get());
        }
    } // namespace nsUI
} // namespace nsApp
