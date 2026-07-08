#include "stdafx.h"
#include "UIScreen.h"

namespace nsApp
{
    namespace nsUI
    {
        nsFramework::Entity& UIScreen::CreateRootEntity()
        {
            rootEntity_ = std::make_unique<nsFramework::Entity>();
            return *rootEntity_;
        }


        nsFramework::Entity& UIScreen::GetRootEntity()
        {
            return *rootEntity_;
        }


        const nsFramework::Entity& UIScreen::GetRootEntity() const
        {
            return *rootEntity_;
        }


        void UIScreen::Update(float deltaTime)
        {
            if (!isVisible_ || rootEntity_ == nullptr)
                return;

            rootEntity_->Update(deltaTime);
        }


        void UIScreen::Draw(RenderContext& rc)
        {
            if (!isVisible_ || rootEntity_ == nullptr)
                return;

            rootEntity_->Draw(rc);
        }


        void UIScreen::SetVisible(bool visible)
        {
            isVisible_ = visible;
        }
    } // namespace nsUI
} // namespace nsApp
