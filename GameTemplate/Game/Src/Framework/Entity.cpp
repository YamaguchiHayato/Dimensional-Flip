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


        void Entity::Update(float deltaTime)
        {
            if (!isEnabled_)
                return;

            transform_.UpdateMatrix();

            for (auto& comp : components_)
                comp->OnUpdate(deltaTime);
        }


        void Entity::Draw(RenderContext& rc)
        {
            if (!isEnabled_)
                return;

            const Matrix& world = transform_.GetWorldMatrix();

            for (auto& comp : components_)
                comp->OnDraw(rc, world);
        }
    } // namespace nsFramework
} // namespace nsApp
