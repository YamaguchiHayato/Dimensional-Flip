#pragma once
#include "Src/Framework/IComponent.h"

/**
 * @file   IUIComponent.h
 * @brief  UI 用 Component の共通インターフェース。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class IUIComponent
         * @brief 設計図の IUIComponent。Framework::IComponent を継承する。
         *
         * OnBuild は OnAttach 直後に 1 回だけ呼ばれる。
         */
        class IUIComponent : public nsFramework::IComponent
        {
        public:
            /**
             * @brief デストラクタ。
             */
            virtual ~IUIComponent() override = default;

            /**
             * @brief Entity 追加時に Build を走らせる。
             * @param pOwner 所有 Entity。
             */
            void OnAttach(nsFramework::Entity* pOwner) override
            {
                nsFramework::IComponent::OnAttach(pOwner);
                if (!isBuilt_)
                {
                    OnBuild();
                    isBuilt_ = true;
                }
            }

            /**
             * @brief 初回構築。テクスチャ・フォント初期化はここで行う。
             */
            virtual void OnBuild() {}


        private:
            bool isBuilt_ = false; //! < Build が完了したかどうか。
        };
    } // namespace nsUI
} // namespace nsApp
