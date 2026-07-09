#pragma once

#include <memory>

#include "Src/Presentation/UI/Core/UIScreenHost.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreen.h"

/**
 * @file   SoundSettingScreenHost.h
 * @brief  SoundSettingScreen を GO として保持する Host。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class SoundSettingScreenHost
         * @brief NewGO して Render パイプラインに載せる Setting UI。
         */
        class SoundSettingScreenHost : public UIScreenHost
        {
        public:
            /**
             * @brief 初期化。SoundSettingData と Screen を接続する。
             * @return 成功なら true。
             */
            bool Start() override;

            /**
             * @brief 具体 UIScreen を取得する。
             * @return SoundSettingScreen。未生成なら nullptr。
             */
            SoundSettingScreen* GetSoundSettingScreen();


        protected:
            /**
             * @brief 具体 UIScreen を生成する。
             * @return SoundSettingScreen。
             */
            std::unique_ptr<UIScreen> CreateScreen() override;
        };
    } // namespace nsUI
} // namespace nsApp
