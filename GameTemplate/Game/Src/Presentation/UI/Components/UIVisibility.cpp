#include "stdafx.h"

#include "Src/Framework/Entity.h"
#include "UIVisibility.h"

namespace nsApp
{
    namespace nsUI
    {
        void UIVisibility::SetVisible(bool visible)
        {
            isVisible_ = visible;
            if (GetOwner() != nullptr)
                GetOwner()->SetEnabled(visible);
        }
    } // namespace nsUI
} // namespace nsApp
