#include "stdafx.h"

#include "PatchNoteOverlayerUI.h"

namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteOverlayUI::Init()
        {
            //! UIImage に定義済みの暗転設定を使う
            image_.InitDimOverlay();
            image_.SetVisible(false);
        }

        void PatchNoteOverlayUI::Update()
        {
            image_.Update();
        }

        void PatchNoteOverlayUI::Draw(RenderContext& rc)
        {
            image_.Draw(rc);
        }

        void PatchNoteOverlayUI::SetVisible(bool visible)
        {
            image_.SetVisible(visible);
        }

        bool PatchNoteOverlayUI::IsVisible() const
        {
            return image_.IsVisible();
        }

        UITransform& PatchNoteOverlayUI::GetTransform()
        {
            return image_.GetTransform();
        }

        UIImage& PatchNoteOverlayUI::GetImage()
        {
            return image_;
        }
    } // namespace nsUI
} // namespace nsApp
