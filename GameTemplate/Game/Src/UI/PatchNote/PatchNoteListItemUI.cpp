#include "stdafx.h"

#include "PatchNoteListItemUI.h"

namespace
{
    constexpr float kTextPaddingX = 28.0f;  //!< 枠左端からの余白。
    constexpr float kBarHalfWidth = 180.0f; //!< バー半幅（360 / 2）。
    constexpr float kTextOffsetY = 6.0f;    //!< バー中心からの微調整（+で上）。

    constexpr float kLineScaleMul = 0.40f; //!< 横1行用フォント倍率。

    //! オレンジ／グレー両方で読みやすい色
    const Vector4 kTextColor = {1.0f, 0.97f, 0.90f, 1.0f};     //!< 薄いクリーム白。
    const Vector4 kShadowColor = {0.12f, 0.08f, 0.06f, 0.85f}; //!< 濃い茶の影。
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteListItemUI::Init(UITransform* parent, const Vector3& localPos, float barScale)
        {
            barScale_ = barScale;

            background_.InitPatchNoteListUnselect();
            background_.GetTransform().SetParent(parent);
            background_.GetTransform().SetLocalPosition(localPos);
            background_.GetTransform().SetLocalScale({barScale_, barScale_, 1.0f});
            background_.GetTransform().SetPivot({0.5f, 0.5f});

            //! 横1行・左寄せ
            lineFont_.SetScale(kLineScaleMul * barScale_);
            lineFont_.SetPivot({0.0f, 0.5f});
            lineFont_.SetColor(kTextColor);
            lineFont_.SetShadowParam(true, 2.5f, kShadowColor);

            loadedSelectArt_ = false;
            RefreshBackground();
        }


        void PatchNoteListItemUI::SetBarPosition(const Vector3& localPos)
        {
            background_.GetTransform().SetLocalPosition(localPos);
        }


        void PatchNoteListItemUI::Update()
        {
            if (!isVisible_)
                return;

            background_.Update();

            const Vector3 bgPos = background_.GetTransform().GetWorldPosition();
            const float textX = bgPos.x - kBarHalfWidth * barScale_ + kTextPaddingX * barScale_;

            //! バー中央付近に横1行で配置
            lineFont_.SetPosition(textX, bgPos.y + kTextOffsetY * barScale_, 0.0f);
        }


        void PatchNoteListItemUI::Draw(RenderContext& rc)
        {
            if (!isVisible_)
                return;

            background_.Draw(rc);
            lineFont_.Draw(rc);
        }


        void PatchNoteListItemUI::SetVisible(bool visible)
        {
            isVisible_ = visible;
            background_.SetVisible(visible);
        }


        void PatchNoteListItemUI::SetSelected(bool selected)
        {
            isSelected_ = selected;
            RefreshBackground();
        }


        void PatchNoteListItemUI::SetEntry(const PatchNoteEntry& entry)
        {
            //! ver とタイトルを横1行に連結（改行なし）
            std::wstring line = entry.version;
            if (!entry.listTitle.empty())
            {
                line += L"  ";
                line += entry.listTitle;
            }

            lineFont_.SetText(line.c_str());
        }


        void PatchNoteListItemUI::RefreshBackground()
        {
            //! 同じ見た目なら Init しない（スティック移動時の再初期化を防ぐ）
            if (isSelected_ == loadedSelectArt_)
                return;

            loadedSelectArt_ = isSelected_;

            const Vector3 pos = background_.GetTransform().GetLocalPosition();
            Transform* parent = background_.GetTransform().GetParent();
            const Vector3 scale = background_.GetTransform().GetLocalScale();

            if (isSelected_)
                background_.InitPatchNoteListSelect();
            else
                background_.InitPatchNoteListUnselect();

            background_.GetTransform().SetParent(parent);
            background_.GetTransform().SetLocalPosition(pos);
            background_.GetTransform().SetLocalScale(scale);
            background_.GetTransform().SetPivot({0.5f, 0.5f});
        }


    } // namespace nsUI
} // namespace nsApp
