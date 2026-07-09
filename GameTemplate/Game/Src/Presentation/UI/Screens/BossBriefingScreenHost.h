#pragma once

#include <memory>

#include "Src/Presentation/UI/Core/UIScreenHost.h"
#include "Src/Presentation/UI/Screens/BossBriefingScreen.h"

/**
 * @file   BossBriefingScreenHost.h
 * @brief  BossBriefingScreen を GO として保持する Host。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossBriefingScreenHost
         * @brief ボス戦チュートリアル UI の GO 境界。
         */
        class BossBriefingScreenHost : public UIScreenHost
        {
        public:
            /**
             * @brief UIScreenHost の Start() をオーバーライドする。
             * @return true: 成功, false: 失敗
             */
            bool Start() override;

            /**
             * @brief 具体 Screen を取得する。
             */
            BossBriefingScreen* GetBossBriefingScreen();


        protected:
            /**
             * @brief UIScreenHost の CreateScreen() をオーバーライドする。
             * @return UIScreen の unique_ptr
             */
            std::unique_ptr<UIScreen> CreateScreen() override;
        };
    } // namespace nsUI
} // namespace nsApp
