#include "stdafx.h"
#include "PatchNotesDimUI.h"


namespace nsApp
{
    namespace nsUI
    {
        void PatchNotesDimUI::Init()
        {
            dim_.Init("Assets/title/Black.DDS", 512.0f, 512.0f);
            dim_.GetTransform().SetAnchorPreset(UIAnchorPreset::Center);
            dim_.GetTransform().SetLocalScale({4.0f, 4.0f, 1.0f});
            dim_.SetColor({0.0f, 0.0f, 0.0f, 0.65f});
            dim_.SetVisible(false);
        }


        void PatchNotesDimUI::Update()
        {
            if (dim_.IsVisible())
                dim_.Update();
        }


        void PatchNotesDimUI::Draw(RenderContext& rc)
        {
            dim_.Draw(rc);
        }


        void PatchNotesDimUI::SetVisible(bool v)
        {
            dim_.SetVisible(v);
        }
    } // namespace nsUI
} // namespace nsApp
