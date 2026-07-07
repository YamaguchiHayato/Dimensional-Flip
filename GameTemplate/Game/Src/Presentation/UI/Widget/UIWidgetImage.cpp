#include "stdafx.h"

#include "Src/Presentation/UI/Components/UISpriteView.h"
#include "UIWidgetImage.h"

namespace nsApp
{
    namespace nsUI
    {
        UISpriteView& UIWidgetImage::AddTo(nsFramework::Entity& entity, const char* path, float width, float height)
        {
            auto& view = entity.AddComponent<UISpriteView>();
            view.Initialize(path, width, height);
            return view;
        }
    } // namespace nsUI
} // namespace nsApp
