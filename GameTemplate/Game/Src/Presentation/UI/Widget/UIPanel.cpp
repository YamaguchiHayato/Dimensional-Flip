#include "stdafx.h"

#include "UIPanel.h"

namespace nsApp
{
    namespace nsUI
    {
        nsFramework::Entity& UIPanel::Create(nsFramework::Entity& parent)
        {
            return parent.CreateChild();
        }
    } // namespace nsUI
} // namespace nsApp
