#pragma once

#include "Src/Presentation/Data/IBossHubData.h"
#include "Src/Presentation/UI/Core/UIScreen.h"

/**
 * @file   BossHudScreen.h
 * @brief  ボス戦 HUD 画面。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossHudScreen
         * @brief ボスアイコン + HP バー UI を構築する UIScreen。
         */
        class BossHudScreen : public UIScreen
        {
        public:
            /**
             * @brief HP データソースを Bind する。
             * @param pData IBossHudData。nullptr 可。
             */
            void Bind(nsPresentation::IBossHudData* pData);

            /**
             * @brief UIScreen::Build() をオーバーライドして、ボス HUD UI を構築する。
             */
            void Build() override;

            /**
             * @brief Bind されたデータソースから現在の HP 割合を取得して、UI に反映する。
             */
            void SyncFromDataSource();


        private:
            nsPresentation::IBossHudData* pHudData_ = nullptr;
            nsFramework::Entity* pLogicEntity_ = nullptr;
        };
    } // namespace nsUI
} // namespace nsApp
