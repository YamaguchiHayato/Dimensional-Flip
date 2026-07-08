#pragma once

#include "UIWidget.h"

/**
 * @file   UIPanel.h
 * @brief  子 Widget をまとめるパネル。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIPanel
         * @brief 子 Entity を 1 パネルとして扱う。
         */
        class UIPanel : public UIWidget
        {
        public:
            /**
             * @brief 親 Entity 配下にパネル用子 Entity を作る。
             * @param parent 親 Entity。
             * @return 生成した子 Entity。
             */
            static nsFramework::Entity& Create(nsFramework::Entity& parent);
        };
    } // namespace nsUI
} // namespace nsApp
