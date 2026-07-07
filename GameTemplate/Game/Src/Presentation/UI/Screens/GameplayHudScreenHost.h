#pragma once

#include "Src/Presentation/UI/Core/UIScreenHost.h"

/**
 * @file   GameplayHudScreenHost.h
 * @brief  GameplayHudScreen を GO として保持する Host。
 */

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
             * @brief 初期化。GameplayHudData と Screen を接続する。
             * @return 成功なら true。
             */
            bool Start() override;

            /**
             * @brief 保持している GameplayHudScreen を取得する。
             * @return GameplayHudScreen。未生成なら nullptr。
             */
            GameplayHudScreen* GetGameplayHudScreen();


        protected:
            /**
             * @brief GameplayHudScreen を生成する。
             * @return GameplayHudScreen の unique_ptr。
             */
            std::unique_ptr<UIScreen> CreateScreen() override;
        };
    } // namespace nsUI
} // namespace nsApp
