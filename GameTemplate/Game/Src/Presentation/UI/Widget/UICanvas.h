#pragma once

#include "UIWidgetElement.h"

/**
 * @file   UICanvas.h
 * @brief  1 画面分の UI ルートキャンバス。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UICanvas
         * @brief UIScreen の root Entity をキャンバスとして扱う。
         */
        class UICanvas : public UIWidgetElement
        {
        public:
            /**
             * @brief ルート Entity にバインドする。
             * @param root UIScreen の root Entity。
             */
            void Initialize(nsFramework::Entity& root);

            /**
             * @brief ルート Entity を取得する。
             * @return root Entity 参照。
             */
            nsFramework::Entity& GetRoot() const;
        };
    } // namespace nsUI
} // namespace nsApp
