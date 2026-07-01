#include "stdafx.h"
#include "PatchNoteWindowUI.h"

namespace
{
    const auto NOTE_POS = Vector3(440.0f, 20.0f, 0.0f); //!< 紙の初期位置（画面中央＋少し下げる）
}

namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteWindowUI::Init()
        {
            /* UIImage に定義済みの紙ウィンドウ設定を使う。*/ 
            image_.InitPatchNoteWindow();

            /* 中央に表示する、*/
            image_.GetTransform().SetAnchorPreset(UIAnchorPreset::Center);

            /* 紙の上端が画面上端に近い位置に来るように Y を少し下げる。*/
            image_.GetTransform().SetLocalPosition(NOTE_POS);

            /* 初期状態は非表示。*/
            image_.SetVisible(false);
        }


        void PatchNoteWindowUI::Update()
        {
            /* 紙の Transform を Sprite に同期する。*/
            image_.Update();
        }


        void PatchNoteWindowUI::Draw(RenderContext& rc)
        {
            /* 紙の Sprite を描画する。*/
            image_.Draw(rc);
        }


        void PatchNoteWindowUI::SetVisible(bool visible)
        {
            /* 表示・非表示を切り替える。*/
            image_.SetVisible(visible);
        }


        UITransform& PatchNoteWindowUI::GetTransform()
        {
            /* 紙の Transform を返す。*/
            return image_.GetTransform();
        }
    } // namespace nsUI
} // namespace nsApp
