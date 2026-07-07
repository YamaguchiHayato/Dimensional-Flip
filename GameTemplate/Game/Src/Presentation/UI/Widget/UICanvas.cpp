#include "stdafx.h"

#include "UICanvas.h"

namespace nsApp
{
    namespace nsUI
    {
        void UICanvas::Initialize(nsFramework::Entity& root)
        {
            Bind(root);
        }


        nsFramework::Entity& UICanvas::GetRoot() const
        {
            return *pEntity_;
        }
    } // namespace nsUI
} // namespace nsApp
