#include "stdafx.h"

#include "PatchNoteListItemUI.h"

namespace
{
    constexpr float kTextPaddingX = 28.0f;
    constexpr float kBarHalfWidth = 180.0f;
    constexpr float kTextOffsetY = 6.0f;
    constexpr float kLineScaleMul = 0.40f;

    const Vector4 kTextColor = {1.0f, 0.97f, 0.90f, 1.0f};
    const Vector4 kShadowColor = {0.12f, 0.08f, 0.06f, 0.85f};
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteListItemUI::Init(UITransform* parent, const Vector3& localPos, float barScale)
        {
            barScale_ = barScale;
            isSelected_ = false;
            isVisible_ = false;

            auto setupBackground = [&](UIImage& bg, void (UIImage::*initFn)())
            {
                (bg.*initFn)();
                bg.GetTransform().SetParent(parent);
                bg.GetTransform().SetLocalPosition(localPos);
                bg.GetTransform().SetLocalScale({barScale_, barScale_, 1.0f});
                bg.GetTransform().SetPivot({0.5f, 0.5f});
                bg.SetVisible(false);
            };

            setupBackground(selectBackground_, &UIImage::InitPatchNoteListSelect);
            setupBackground(unselectBackground_, &UIImage::InitPatchNoteListUnselect);

            lineFont_.SetScale(kLineScaleMul * barScale_);
            lineFont_.SetPivot({0.0f, 0.5f});
            lineFont_.SetColor(kTextColor);
            lineFont_.SetShadowParam(true, 2.5f, kShadowColor);

            ApplyBackgroundVisibility();
        }

        void PatchNoteListItemUI::SetBarPosition(const Vector3& localPos)
        {
            selectBackground_.GetTransform().SetLocalPosition(localPos);
            unselectBackground_.GetTransform().SetLocalPosition(localPos);
        }

        void PatchNoteListItemUI::Update()
        {
            if (!isVisible_)
                return;

            selectBackground_.Update();
            unselectBackground_.Update();

            const Vector3 bgPos = unselectBackground_.GetTransform().GetWorldPosition();
            const float textX = bgPos.x - kBarHalfWidth * barScale_ + kTextPaddingX * barScale_;

            lineFont_.SetPosition(textX, bgPos.y + kTextOffsetY * barScale_, 0.0f);
        }

        void PatchNoteListItemUI::Draw(RenderContext& rc)
        {
            if (!isVisible_)
                return;

            selectBackground_.Draw(rc);
            unselectBackground_.Draw(rc);
            lineFont_.Draw(rc);
        }

        void PatchNoteListItemUI::SetVisible(bool visible)
        {
            isVisible_ = visible;
            ApplyBackgroundVisibility();
        }

        void PatchNoteListItemUI::SetSelected(bool selected)
        {
            if (isSelected_ == selected)
                return;

            isSelected_ = selected;
            ApplyBackgroundVisibility();
        }

        void PatchNoteListItemUI::SetEntry(const PatchNoteEntry& entry)
        {
            std::wstring line = entry.version;
            if (!entry.listTitle.empty())
            {
                line += L"  ";
                line += entry.listTitle;
            }
            lineFont_.SetText(line.c_str());
        }

        void PatchNoteListItemUI::ApplyBackgroundVisibility()
        {
            const bool show = isVisible_;
            selectBackground_.SetVisible(show && isSelected_);
            unselectBackground_.SetVisible(show && !isSelected_);
        }
    } // namespace nsUI
} // namespace nsApp
