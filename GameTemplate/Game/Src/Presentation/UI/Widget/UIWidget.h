#pragma once

#include "UIWidgetElement.h"

/**
 * @file   UIWidget.h
 * @brief  インタラクティブ / 描画可能な UI 要素の基底。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIWidget
         * @brief UIWidgetImage / UILabel / UIPanel の共通基底。
         */
        class UIWidget : public UIWidgetElement
        {
        };
    } // namespace nsUI
} // namespace nsApp
