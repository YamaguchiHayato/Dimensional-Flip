#pragma once

#include "UIWidget.h"

/**
 * @file   UIWidgetImage.h
 * @brief  画像表示 Widget（Presentation 層）。
 */

namespace nsApp
{
    namespace nsUI
    {
        class UISpriteView;

        /**
         * @class UIWidgetImage
         * @brief 画像表示 Widget。内部で UISpriteView を使う。
         * @note  旧 Src/UI/Framework/UIImage とは別クラス。
         */
        class UIWidgetImage : public UIWidget
        {
        public:
            /**
             * @brief Entity にスプライト Component を追加して初期化する。
             * @param entity 対象 Entity。
             * @param path テクスチャパス。
             * @param width 幅。
             * @param height 高さ。
             * @return 追加した UISpriteView。
             */
            static UISpriteView& AddTo(nsFramework::Entity& entity, const char* path, float width, float height);
        };
    } // namespace nsUI
} // namespace nsApp
