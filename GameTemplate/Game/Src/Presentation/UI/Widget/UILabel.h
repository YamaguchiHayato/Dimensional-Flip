#pragma once

#include "UIWidget.h"

/**
 * @file   UILabel.h
 * @brief  テキスト表示 Widget。
 */

namespace nsApp
{
    namespace nsUI
    {
        class UITextView;

        /**
         * @class UILabel
         * @brief テキスト表示 Widget。内部で UITextView を 1 つ持つ。
         */
        class UILabel : public UIWidget
        {
        public:
            /**
             * @brief Entity に UILabel 用 UITextView を追加する。
             * @param entity 対象 Entity。
             * @return 追加した UITextView。
             */
            static UITextView& AddTo(nsFramework::Entity& entity);
        };
    } // namespace nsUI
} // namespace nsApp
