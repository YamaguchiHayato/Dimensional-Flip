#include "stdafx.h"
#include "GameplayHudScreen.h"
#include "GameplayHudScreenHost.h"

namespace nsApp
{
    namespace nsUI
    {
        GameplayHudScreen* GameplayHudScreenHost::GetGameplayHudScreen()
        {
            return dynamic_cast<GameplayHudScreen*>(screen_.get());
        }
    } // namespace nsUI
} // namespace nsApp
