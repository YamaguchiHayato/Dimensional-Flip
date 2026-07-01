#include "stdafx.h"

#include "PatchNoteTitleBarUI.h"

namespace
{
    constexpr float kBannerLocalX = 60.0f;  //! 右へ＋、左へ－
    constexpr float kBannerLocalY = 400.0f; //! 上へ＋、下へ－
    constexpr float kTitleOffsetX = -190.0f; // 左へ（マイナス）
    constexpr float kTitleOffsetY = 45.0f;   // 上下（+で上）
    constexpr float kTitleScale = 1.2f;

    const auto PIVOT = Vector2{0.5f, 0.5f}; //!< 中心
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteTitleBarUI::Init(UITransform* parent)
        {
            /* 青色リボンの位置を設定。*/
            banner_.InitPatchNoteBanner();
            banner_.GetTransform().SetParent(parent);
            banner_.GetTransform().SetLocalPosition({kBannerLocalX, kBannerLocalY, 0.0f});
            banner_.GetTransform().SetPivot({PIVOT});

            /* タイトル文字の初期設定。*/
            titleFont_.SetText(L"Patch Note");
            titleFont_.SetScale(kTitleScale);
            titleFont_.SetPivot({PIVOT});
            titleFont_.SetShadowParam(true, 2.0f, Vector4::Black);
        }


        void PatchNoteTitleBarUI::Update()
        {
            /* 表示されていなければ更新しない。*/
            if (!isVisible_)
                return;

            /* リボンの更新。*/
            banner_.Update();

            /* タイトル文字の位置をリボンの上に設定。*/
            const Vector3 bannerPos = banner_.GetTransform().GetWorldPosition();

            /* タイトル文字の位置をリボンの上に設定。*/
            titleFont_.SetPosition(bannerPos.x + kTitleOffsetX, bannerPos.y + kTitleOffsetY, 0.0f);
        }


        void PatchNoteTitleBarUI::Draw(RenderContext& rc)
        {
            /* 表示されていなければ描画しない。*/
            if (!isVisible_)
                return;

            /* リボンとタイトル文字を描画。*/
            banner_.Draw(rc);
            titleFont_.Draw(rc);
        }


        void PatchNoteTitleBarUI::SetVisible(bool visible)
        {
            /* 表示フラグを設定し、リボンの表示状態も更新。*/
            isVisible_ = visible;
            banner_.SetVisible(visible);
        }


        void PatchNoteTitleBarUI::SetTitleText(const wchar_t* text)
        {
            /* タイトル文字を設定。*/
            titleFont_.SetText(text);
        }
    } // namespace nsUI
} // namespace nsApp
