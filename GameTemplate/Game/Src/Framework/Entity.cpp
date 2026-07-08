#include "stdafx.h"

#include "Entity.h"

namespace nsApp
{
    namespace nsFramework
    {
        Entity::Entity(TransformNode* pParentTransform)
        {
            transform_.SetParent(pParentTransform);
        }


        Entity& Entity::CreateChild()
        {
            auto child = std::make_unique<Entity>(&transform_);
            Entity& ref = *child;
            children_.push_back(std::move(child));
            return ref;
        }


        void Entity::Update(float deltaTime)
        {
            /* 有効化されていない場合は、Updateをスキップする。 */
            if (!isEnabled_)
                return;

            /* Transformのワールド行列を更新する。 */
            transform_.UpdateMatrix();

            /* ComponentのUpdateを呼び出す。 */
            for (auto& comp : components_)
                comp->OnUpdate(deltaTime);

            /* 子EntityのUpdateを呼び出す。 */
            for (auto& child : children_)
                child->Update(deltaTime);
        }


        void Entity::Draw(RenderContext& rc)
        {
            /* 有効化されていない場合は、Drawをスキップする。 */
            if (!isEnabled_)
                return;

            /* Transformのワールド行列を取得する。 */
            const Matrix& world = transform_.GetWorldMatrix();

            /* ComponentのDrawを呼び出す。 */
            for (auto& comp : components_)
                comp->OnDraw(rc, world);

            /* 子EntityのDrawを呼び出す。 */
            for (auto& child : children_)
                child->Draw(rc);
        }
    } // namespace nsFramework
} // namespace nsApp
