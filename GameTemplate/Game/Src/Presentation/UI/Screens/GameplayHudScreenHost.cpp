#include "stdafx.h"

#include "GameplayHudScreen.h"
#include "GameplayHudScreenHost.h"
#include "Src/Core/Game.h"
#include "Src/Presentation/Data/GameplayHudData.h"

namespace nsApp
{
    namespace nsUI
    {
        bool GameplayHudScreenHost::Start()
        {
            if (!UIScreenHost::Start())
                return false;

            auto* pScreen = GetGameplayHudScreen();
            auto* pGame = FindGO<nsCore::Game>("game");
            if (pScreen == nullptr || pGame == nullptr)
                return true;

            if (auto* pData = pGame->GetGameplayHudData())
                pScreen->ConnectToData(pData);

            return true;
        }


        GameplayHudScreen* GameplayHudScreenHost::GetGameplayHudScreen()
        {
            return dynamic_cast<GameplayHudScreen*>(screen_.get());
        }


        std::unique_ptr<UIScreen> GameplayHudScreenHost::CreateScreen()
        {
            return std::make_unique<GameplayHudScreen>();
        }
    } // namespace nsUI
} // namespace nsApp
