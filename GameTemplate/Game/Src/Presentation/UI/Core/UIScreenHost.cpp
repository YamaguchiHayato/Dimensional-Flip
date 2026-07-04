#include "stdafx.h"

#include "UIScreenHost.h"

namespace nsApp
{
    namespace nsUI
    {
        bool UIScreenHost::Start()
        {
            screen_ = CreateScreen();
            if (screen_ == nullptr)
                return false;

            /* 初回 Build で UI ツリーを構築する */
            screen_->Build();
            return true;
        }


        void UIScreenHost::Update()
        {
            if (screen_ == nullptr)
                return;

            float dt = g_gameTime->GetFrameDeltaTime();
            screen_->Update(dt);
        }


        void UIScreenHost::Render(RenderContext& rc)
        {
            if (screen_ == nullptr)
                return;

            screen_->Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
