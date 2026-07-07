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

            /* Game 内の GameplayHudData と Screen を接続 */
            if (auto* pData = pGame->GetGameplayHudData())
                pScreen->ConnectToData(pData);

            return true;
        }


        GameplayHudScreen* GameplayHudScreenHost::GetGameplayHudScreen()
        {
            return dynamic_cast<GameplayHudScreen*>(screen_.get());
        }


        void GameplayHudScreenHost::Render(RenderContext& rc)
        {
            auto* pScreen = GetGameplayHudScreen();
            if (pScreen == nullptr)
                return;

            /* UIScreenHost::Render は screen_->Draw() だが、
               View 直描画なので DrawHud を直接呼ぶ */
            pScreen->DrawHud(rc);
        }


        std::unique_ptr<UIScreen> GameplayHudScreenHost::CreateScreen()
        {
            return std::make_unique<GameplayHudScreen>();
        }
    } // namespace nsUI
} // namespace nsApp
