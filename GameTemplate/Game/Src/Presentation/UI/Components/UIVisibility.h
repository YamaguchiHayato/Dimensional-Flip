#pragma once
#include "IUIComponent.h"

/**
 * @file   UIVisibility.h
 * @brief  所有 Entity の表示 / 非表示を制御する Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIVisibility
         * @brief Entity::SetEnabled をラップする可視制御 Component。
         */
        class UIVisibility : public IUIComponent
        {
        public:
            /**
             * @brief 表示 / 非表示を設定する。
             * @param visible true で表示。
             */
            void SetVisible(bool visible);

            /**
             * @brief 現在の表示状態を返す。
             * @return 表示中なら true。
             */
            bool IsVisible() const { return isVisible_; }


        private:
            bool isVisible_ = true; //! < 表示状態。true で表示中。
        };
    } // namespace nsUI
} // namespace nsApp
