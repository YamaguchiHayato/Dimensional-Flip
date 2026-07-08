#include "stdafx.h"

#include "Src/Presentation/UI/Components/UITextView.h"
#include "UILabel.h"

namespace nsApp
{
    namespace nsUI
    {
        UITextView& UILabel::AddTo(nsFramework::Entity& entity)
        {
            return entity.AddComponent<UITextView>();
        }
    } // namespace nsUI
} // namespace nsApp
