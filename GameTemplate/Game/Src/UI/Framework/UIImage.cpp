#include "stdafx.h"

#include "UIImage.h"

namespace nsApp
{
    namespace nsUI
    {
        void UIImage::Init(const char* texturePath, float width, float height)
        {
            sprite_.Init(texturePath, width, height);
            transform_.SetSize({width, height});
            isInitialized_ = true;
        }

        void UIImage::InitFromUIAsset(const std::string& uiName, float width, float height)
        {
            const std::string path = "Assets/UI/" + uiName + ".DDS";
            Init(path.c_str(), width, height);
        }

        void UIImage::InitFromAssetDef(const UIImageAssetDef& assetDef)
        {
            Init(assetDef.path, assetDef.width, assetDef.height);
        }

        void UIImage::InitPatchNoteWindow()
        {
            InitFromAssetDef(UIImageAsset::PatchNoteWindow);
        }

        void UIImage::InitPatchNoteBanner()
        {
            InitFromAssetDef(UIImageAsset::PatchNoteBanner);
        }

        void UIImage::InitPatchNoteListSelect()
        {
            InitFromAssetDef(UIImageAsset::PatchNoteListSelect);
        }

        void UIImage::InitPatchNoteListUnselect()
        {
            InitFromAssetDef(UIImageAsset::PatchNoteListUnselect);
        }

        void UIImage::InitPatchNoteBadge()
        {
            InitFromAssetDef(UIImageAsset::PatchNoteBadge);
        }

        void UIImage::InitDimOverlay()
        {
            InitFromAssetDef(UIImageAsset::TitleBlack);
            GetTransform().SetAnchorPreset(UIAnchorPreset::Center);
            GetTransform().SetLocalScale({4.0f, 4.0f, 1.0f});
            SetColor({0.0f, 0.0f, 0.0f, 0.65f});
        }

        void UIImage::InitButtonB()
        {
            InitFromAssetDef(UIImageAsset::ButtonB);
        }


        void UIImage::InitButtonMenu()
        {
            InitFromAssetDef(UIImageAsset::ButtonMenu);
        }


        void UIImage::Update()
        {
            if (!isInitialized_ || !isVisible_)
                return;

            transform_.ApplyTo(sprite_);
            sprite_.Update();
        }

        void UIImage::Draw(RenderContext& rc)
        {
            if (!isInitialized_ || !isVisible_)
                return;

            sprite_.Draw(rc);
        }

        void UIImage::SetColor(const Vector4& color)
        {
            sprite_.SetMulColor(color);
        }
    } // namespace nsUI
} // namespace nsApp
