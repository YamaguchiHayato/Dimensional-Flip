#include "stdafx.h"

#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"

namespace nsApp
{
    namespace nsUI
    {
        std::unique_ptr<UIScreen> BossHudScreenHost::CreateScreen()
        {
            return std::make_unique<BossHudScreen>();
        }


        BossHudScreen* BossHudScreenHost::GetBossHudScreen()
        {
            return dynamic_cast<BossHudScreen*>(screen_.get());
        }
    } // namespace nsUI
} // namespace nsApp
