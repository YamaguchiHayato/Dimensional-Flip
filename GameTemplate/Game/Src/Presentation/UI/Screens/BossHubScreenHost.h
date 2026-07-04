#pragma once

#include <memory>

#include "Src/Presentation/UI/Core/UIScreenHost.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"

/**
 * @file   BossHudScreenHost.h
 * @brief  ボス HUD 用 UIScreenHost GO。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossHudScreenHost
         * @brief NewGO して Render パイプラインに載せるボス HUD。
         */
        class BossHudScreenHost : public UIScreenHost
        {
        public:
            /**
             * @brief 具体 UIScreen を生成する。
             * @return BossHudScreen。
             */
            BossHudScreen* GetBossHudScreen();

        protected:
            std::unique_ptr<UIScreen> CreateScreen() override;
        };
    } // namespace nsUI
} // namespace nsApp
