#include "stdafx.h"

#include "SoundSettingScreenHost.h"
#include "Src/Core/Game.h"
#include "Src/Presentation/Data/SoundSettingData.h"

namespace nsApp
{
    namespace nsUI
    {
        bool SoundSettingScreenHost::Start()
        {
            if (!UIScreenHost::Start())
                return false;

            auto* pUiScreen = GetScreen();
            auto* pGame = FindGO<nsCore::Game>("game");
            if (pUiScreen == nullptr || pGame == nullptr)
                return true;

            auto* pScreen = static_cast<SoundSettingScreen*>(pUiScreen);

            /* Data と Screen を接続する。 */
            if (auto* pData = pGame->GetSoundSettingData())
            {
                pData->SetScreen(pScreen);
                pScreen->Bind(pData);
            }

            /* 起動時は非表示。ポーズの Setting から開く。 */
            pScreen->SetVisible(false);
            return true;
        }


        std::unique_ptr<UIScreen> SoundSettingScreenHost::CreateScreen()
        {
            return std::make_unique<SoundSettingScreen>();
        }


        SoundSettingScreen* SoundSettingScreenHost::GetSoundSettingScreen()
        {
            if (screen_ == nullptr)
                return nullptr;

            return static_cast<SoundSettingScreen*>(screen_.get());
        }
    } // namespace nsUI
} // namespace nsApp
