#include "stdafx.h"

#include "TitlePatchNoteHintUI.h"

namespace
{
    constexpr float kMenuTexWidth = 128.0f;
    constexpr float kMenuTexHeight = 128.0f;

    //! 画面左端（1920 基準で -960）。少し内側に余白
    constexpr float kLeftMarginX = -900.0f;
    constexpr float kHintBaseY = -400.0f; //!< Copyright 付近の高さ

    constexpr float kMenuScreenX = kLeftMarginX;
    constexpr float kMenuScreenY = kHintBaseY;
    constexpr float kMenuScale = 0.50f;

    //! Menu の右に Information（アイコン幅 + 隙間）
    constexpr float kInfoTextGapX = 72.0f;
    constexpr float kInfoTextScreenX = kLeftMarginX + kInfoTextGapX;
    constexpr float kInfoTextScreenY = kHintBaseY + 6.0f;
    constexpr float kInfoTextScale = 1.1f;

    const auto kMenuPivot = Vector2{0.0f, 0.5f}; //!< 左中央
    const auto kInfoTextPivot = Vector2{0.0f, 0.5f};
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void TitlePatchNoteHintUI::Init(UITransform* parent)
        {
            //! Menu 画像（patcNote/Menu.DDS）
            menu_.Init("Assets/UI/patcNote/Menu.DDS", kMenuTexWidth, kMenuTexHeight);
            menu_.GetTransform().SetParent(parent);
            menu_.GetTransform().SetLocalPosition({kMenuScreenX, kMenuScreenY, 0.0f});
            menu_.GetTransform().SetLocalScale({kMenuScale, kMenuScale, 1.0f});
            menu_.GetTransform().SetPivot({kMenuPivot});

            //! Information 文字
            infoText_.SetText(L"Information");
            infoText_.SetScale(kInfoTextScale);
            infoText_.SetPivot({kInfoTextPivot});
            infoText_.SetColor(Vector4::White);
            infoText_.SetShadowParam(true, 2.0f, Vector4::Black);
        }


        void TitlePatchNoteHintUI::Update()
        {
            /* 表示フラグが経たない場合は非表示に。*/
            if (!isVisible_)
                return;

            /* Menu 画像の更新。*/
            menu_.Update();

            /* Information 文字の更新。*/
            infoText_.SetPosition(kInfoTextScreenX, kInfoTextScreenY, 0.0f);
        }


        void TitlePatchNoteHintUI::Draw(RenderContext& rc)
        {
            /* 表示フラグが経たない場合は非表示に。*/
            if (!isVisible_)
                return;

            /* Menu 画像の描画。*/
            menu_.Draw(rc);
            infoText_.Draw(rc);
        }


        void TitlePatchNoteHintUI::SetVisible(bool visible)
        {
            /* 表示フラグを更新。*/
            isVisible_ = visible;

            /* Menu 画像の表示フラグを更新。*/
            menu_.SetVisible(visible);
        }
    } // namespace nsUI
} // namespace nsApp
