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
             * @brief 初期化。BossHudData と Screen を接続する。
             * @return 成功なら true。
             */
            bool Start() override;

            /**
             * @brief 具体 UIScreen を取得する。
             * @return BossHudScreen。未生成なら nullptr。
             */
            BossHudScreen* GetBossHudScreen();


        protected:
            /**
             * @brief 具体 UIScreen を生成する。
             * @return BossHudScreen。
             */
            std::unique_ptr<UIScreen> CreateScreen() override;
        };
    } // namespace nsUI
} // namespace nsApp
