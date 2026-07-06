#pragma once

#include "Src/Presentation/UI/Core/UIScreenHost.h"

namespace nsApp
{
    namespace nsUI
    {
        class GameplayHudScreen;

        /**
         * @class GameplayHudScreenHost
         * @brief GameplayHudScreen を GO として保持する Host。
         */
        class GameplayHudScreenHost : public UIScreenHost
        {
        public:
            /**
             * @brief GameplayHudScreen を取得する。
             * @return GameplayHudScreen のポインタ。
             */
            GameplayHudScreen* GetGameplayHudScreen();
        };
    } // namespace nsUI
} // namespace nsApp
