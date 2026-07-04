#pragma once

#include <memory>
#include <vector>

#include "IComponent.h"
#include "TransformNode.h"

/**
 * @file   Entity.h
 * @brief  Transform と Component を束ねる Framework の中核。
 * @note   GO ではない。Boss や UIScreen が所有する。
 */

namespace nsApp
{
    namespace nsFramework
    {
        /**
         * @class Entity
         * @brief Component コンテナ。Update / Draw を各 Component に委譲する。
         */
        class Entity
        {
        public:
            Entity() = default;
            explicit Entity(TransformNode* pParentTransform);

            /**
             * @brief Transform を取得する。
             * @return Transform 参照。
             */
            TransformNode& GetTransform() { return transform_; }
            const TransformNode& GetTransform() const { return transform_; }

            /**
             * @brief Component を生成して追加する。
             * @tparam T Component 型。
             * @tparam Args コンストラクタ引数。
             * @return 追加した Component 参照。
             */
            template <typename T, typename... Args> T& AddComponent(Args&&... args)
            {
                auto component = std::make_unique<T>(std::forward<Args>(args)...);
                T& ref = *component;
                component->OnAttach(this);
                components_.push_back(std::move(component));
                return ref;
            }

            /**
             * @brief 指定型の Component を取得する。
             * @tparam T Component 型。
             * @return 見つかった Component。なければ nullptr。
             */
            template <typename T> T* GetComponent() const
            {
                for (const auto& comp : components_)
                {
                    if (auto* p = dynamic_cast<T*>(comp.get()))
                        return p;
                }
                return nullptr;
            }

            /**
             * @brief 有効フラグを設定する。
             * @param enabled false で Update / Draw を止める。
             */
            void SetEnabled(bool enabled) { isEnabled_ = enabled; }

            /**
             * @brief 有効かどうか。
             * @return 有効なら true。
             */
            bool IsEnabled() const { return isEnabled_; }

            /**
             * @brief Transform と Component を更新する。
             * @param deltaTime 経過秒数。
             */
            void Update(float deltaTime);

            /**
             * @brief Component を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Draw(struct RenderContext& rc);

        private:
            TransformNode transform_;
            std::vector<std::unique_ptr<IComponent>> components_;
            bool isEnabled_ = true;
        };
    } // namespace nsFramework
} // namespace nsApp
