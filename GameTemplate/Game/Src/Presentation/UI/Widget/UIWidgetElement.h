#pragma once

#include "Src/Framework/Entity.h"

/**
 * @file   UIWidgetElement.h
 * @brief  Presentation 層 UI ツリーの基底。Entity を 1 ノードとして束ねる。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIWidgetElement
         * @brief 設計図の UIElement。Entity への参照を保持する。
         * @note  旧 Src/UI/Framework/UIElement とは別クラス。
         */
        class UIWidgetElement
        {
        public:
            /**
             * @brief デフォルトコンストラクタ。
             */
            UIWidgetElement() = default;

            /**
             * @brief バインドする Entity を設定する。
             * @param entity 対象 Entity。
             */
            void Bind(nsFramework::Entity& entity) { pEntity_ = &entity; }

            /**
             * @brief バインド済み Entity を取得する。
             * @return Entity ポインタ。未バインドなら nullptr。
             */
            nsFramework::Entity* GetEntity() const { return pEntity_; }

            /**
             * @brief Transform を取得する。
             * @return Transform 参照。
             */
            nsFramework::TransformNode& GetTransform() { return pEntity_->GetTransform(); }

        protected:
            nsFramework::Entity* pEntity_ = nullptr;
        };
    } // namespace nsUI
} // namespace nsApp
