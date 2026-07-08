#pragma once

#include <memory>

#include "Entity.h"

/**
 * @file   EntityHost.h
 * @brief  Entity の所有と Update / Draw 委譲を行う薄いヘルパ。
 * @note  Boss や UIScreenHost からメンバとして使う。
 */

namespace nsApp
{
    namespace nsFramework
    {
        /**
         * @class EntityHost
         * @brief Root Entity を1つ保持し、更新・描画入口を提供する。
         */
        class EntityHost
        {
        public:
            EntityHost() = default;

            /**
             * @brief Root Entity を生成する。
             * @return 生成した Root Entity 参照。
             */
            Entity& CreateRootEntity()
            {
                rootEntity_ = std::make_unique<Entity>();
                return *rootEntity_;
            }

            /**
             * @brief Root Entity を取得する。
             * @return Root Entity。未生成なら nullptr。
             */
            Entity* GetRootEntity() { return rootEntity_.get(); }
            const Entity* GetRootEntity() const { return rootEntity_.get(); }

            /**
             * @brief Root Entity を更新する。
             * @param deltaTime 経過秒数。
             */
            void Update(float deltaTime)
            {
                if (rootEntity_ != nullptr)
                    rootEntity_->Update(deltaTime);
            }

            /**
             * @brief Root Entity を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Draw(RenderContext& rc)
            {
                if (rootEntity_ != nullptr)
                    rootEntity_->Draw(rc);
            }

            /**
             * @brief Root Entity を破棄する。
             */
            void Destroy()
            {
                if (rootEntity_ != nullptr)
                {
                    for (auto& comp : /* 必要なら明示 Detach */ (void) 0, rootEntity_)
                    {
                    }
                    rootEntity_.reset();
                }
            }

        private:
            std::unique_ptr<Entity> rootEntity_;
        };
    } // namespace nsFramework
} // namespace nsApp
