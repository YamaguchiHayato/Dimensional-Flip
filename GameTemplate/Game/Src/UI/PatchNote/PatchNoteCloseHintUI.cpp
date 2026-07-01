#include "stdafx.h"

#include "PatchNoteCloseHintUI.h"

namespace
{
    //-------------------------------------------------------------------------
    // 座標はすべて「紙ウィンドウ（note.DDS）のローカル座標」。
    // 原点 = 紙の中心。+X = 右、+Y = 上。
    //
    // WorldSelectUI / ManualUI と同様、B ボタンと Back は別々に座標を置く。
    // btnPos からの相対オフセットでは ButtonB.DDS の絵のズレを吸収しきれない。
    //-------------------------------------------------------------------------

    //! B ボタン画像の位置（紙ローカル）
    constexpr float kButtonLocalX = -1050.0f; //!< 小さく＝左へ、大きく＝右へ
    constexpr float kButtonLocalY = -450.0f;  //!< 大きく（0 に近く）＝上へ、小さく＝下へ
    constexpr float kButtonScale = 0.16f;     //!< ボタン画像の拡大率

    //! Back 文字の位置（紙ローカル・B ボタンとは独立）
    constexpr float kBackTextLocalX = -1105.0f; //!< 小さく＝左へ（B の左側）
    constexpr float kBackTextLocalY = -420.0f;  //!< 高さ合わせ用（-450 + 65 が目安）
    constexpr float kBackTextScale = 1.0f;      //!< Back 文字の拡大率

    //! ピボット（WorldSelectUI / ManualUI と同じ右中央）
    const auto kButtonPivot = Vector2{1.0f, 0.5f};
    const auto kBackTextPivot = Vector2{1.0f, 0.5f};
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteCloseHintUI::Init(UITransform* parent)
        {
            parent_ = parent;

            //! B ボタン画像を初期化して紙の子にする
            button_.InitButtonB();
            button_.GetTransform().SetParent(parent_);
            button_.GetTransform().SetLocalPosition({kButtonLocalX, kButtonLocalY, 0.0f});
            button_.GetTransform().SetLocalScale({kButtonScale, kButtonScale, 1.0f});
            button_.GetTransform().SetPivot({kButtonPivot});

            //! Back 文字の見た目だけ設定（位置は Update で紙基準に置く）
            backText_.SetText(L"Back");
            backText_.SetScale(kBackTextScale);
            backText_.SetPivot({kBackTextPivot});
            backText_.SetColor(Vector4::White);
            backText_.SetShadowParam(true, 2.0f, Vector4::Black);
        }


        void PatchNoteCloseHintUI::Update()
        {
            /* 表示状態が false なら更新しない。*/
            if (!isVisible_)
                return;

            /* ボタンUIの更新。*/
            button_.Update();

            /* Back 文字の位置を紙ウィンドウのワールド座標に合わせる。*/
            const Vector3 paperPos = parent_->GetWorldPosition();

            /* Back 文字のワールド座標を紙ウィンドウのワールド座標に合わせる。*/
            backText_.SetPosition(paperPos.x + kBackTextLocalX, paperPos.y + kBackTextLocalY, 0.0f);
        }


        void PatchNoteCloseHintUI::Draw(RenderContext& rc)
        {
            /* 表示状態が false なら描画しない。*/
            if (!isVisible_)
                return;

            /* ボタンと Back 文字を描画。*/
            button_.Draw(rc);
            backText_.Draw(rc);
        }


        void PatchNoteCloseHintUI::SetVisible(bool visible)
        {
            /* 表示状態を更新。*/
            isVisible_ = visible;
            button_.SetVisible(visible);
        }
    } // namespace nsUI
} // namespace nsApp
