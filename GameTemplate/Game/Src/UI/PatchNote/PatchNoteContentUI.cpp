#include "stdafx.h"

#include "PatchNoteContentUI.h"

namespace
{
    //! --- 小見出し（紙の中央・大きく）---
    constexpr float kHeaderLocalX = -300.0f;   //!< 0 = 紙の横中央
    constexpr float kHeaderLocalY = 300.0f; //!< 上からの位置（+で上）
    constexpr float kHeaderScale = 1.20f;   //!< 小見出しの大きさ

    //! --- 詳細（小見出しの下・左揃え）---
    constexpr float kBodyLocalX = -400.0f;    //!< 左マージン（小さく＝左へ）
    constexpr float kBodyLocalBaseY = 200.0f; // 見出しが上がった分、少し下げて間隔確保
    constexpr float kBodyLineStepY = 34.0f;  // 本文を大きくしたら行間も少し広げる
    constexpr float kBodyScale = 0.68f;      // 0.55 → 0.68

    constexpr float kBodyLineStepY_ = 40.0f; //!< 行間。
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteContentUI::Init(UITransform* parent)
        {
            /* 親追従処理にする。*/
            parent_ = parent;

            /* デフォルトスタイルを適用する。*/
            ApplyDefaultStyle();
        }


        void PatchNoteContentUI::ApplyDefaultStyle()
        {
            /* デフォルトの位置・スケール・色・影を設定する。*/
            headerLocalPos_ = {kHeaderLocalX, kHeaderLocalY, 0.0f};
            bodyLocalX_ = kBodyLocalX;
            bodyLocalBaseY_ = kBodyLocalBaseY;
            bodyLineStepY_ = kBodyLineStepY;

            /* ヘッダーのスタイルを設定する。*/
            headerFont_.SetScale(kHeaderScale);
            headerFont_.SetPivot({0.5f, 0.5f}); //!< 中央揃え
            headerFont_.SetColor(Vector4::Black);
            headerFont_.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 0.5f));

            /* 本文のスタイルを設定する。*/
            for (int i = 0; i < MAX_BODY_LINES; ++i)
            {
                bodyLines_[i].SetScale(kBodyScale);
                bodyLines_[i].SetPivot({0.0f, 0.5f}); //!< 左揃え
                bodyLines_[i].SetColor(Vector4(0.15f, 0.15f, 0.18f, 1.0f));
                bodyLines_[i].SetShadowParam(false, 0.0f, Vector4::Black);
                bodyLines_[i].SetText(L"");
            }

            /* 初期位置を適用する。*/
            ApplyWorldPositions();
        }


        void PatchNoteContentUI::Update()
        {
            /* 親 Transform のワールド座標に追従する。*/
            ApplyWorldPositions();
        }


        void PatchNoteContentUI::ApplyWorldPositions()
        {
            /* 親 Transform が nullptr なら何もしない。*/
            if (parent_ == nullptr)
                return;

            /* 親 Transform のワールド座標を取得する。*/
            const Vector3 parentPos = parent_->GetWorldPosition();

            /* ヘッダーと本文のワールド座標を設定する。*/
            headerFont_.SetPosition(parentPos.x + kHeaderLocalX, parentPos.y + kHeaderLocalY, 0.0f);

            /* 本文の各行のワールド座標を設定する。*/
            for (int i = 0; i < MAX_BODY_LINES; ++i)
            {
                /* 本文の各行の Y 座標を計算する。*/
                const float localY = kBodyLocalBaseY - i * kBodyLineStepY_;
                bodyLines_[i].SetPosition(parentPos.x + kBodyLocalX, parentPos.y + localY, 0.0f);
            }
        }


        void PatchNoteContentUI::Draw(RenderContext& rc)
        {
            /* 表示フラグが false なら描画しない。*/
            if (!isVisible_)
                return;

            /* ヘッダーと本文を描画する。*/
            headerFont_.Draw(rc);

            /* 本文は改行で分割して FontRender に設定済みなので、各行を描画する。*/
            for (int i = 0; i < MAX_BODY_LINES; ++i)
                bodyLines_[i].Draw(rc);
        }


        void PatchNoteContentUI::SetVisible(bool visible)
        {
            /* 表示フラグを設定する。*/
            isVisible_ = visible;
        }

        void PatchNoteContentUI::SetFromEntry(const PatchNoteEntry& entry)
        {
            /* パッチノートエントリからヘッダーと本文を設定する。*/
            SetHeaderText(entry.title.c_str());

            /* 本文は改行で分割して FontRender に設定する。*/
            SetBodyText(entry.body.c_str());
        }


        void PatchNoteContentUI::SetHeaderText(const wchar_t* text)
        {
            /* ヘッダーのテキストを設定する。*/
            headerFont_.SetText(text);
        }


        void PatchNoteContentUI::SetBodyText(const wchar_t* text)
        {
            /* 本文のテキストを設定する。改行で分割して FontRender に設定する。*/
            SplitBodyToLines(text ? std::wstring(text) : std::wstring());
        }


        void PatchNoteContentUI::SetHeaderScale(float scale)
        {
            /* ヘッダーのスケールを設定する。*/
            headerFont_.SetScale(scale);
        }


        void PatchNoteContentUI::SetBodyScale(float scale)
        {
            /* 本文のスケールを設定する。*/
            for (int i = 0; i < MAX_BODY_LINES; ++i)
                bodyLines_[i].SetScale(scale);
        }


        void PatchNoteContentUI::SetHeaderColor(const Vector4& color)
        {
            /* ヘッダーの色を設定する。*/
            headerFont_.SetColor(color);
        }


        void PatchNoteContentUI::SetBodyColor(const Vector4& color)
        {
            /* 本文の色を設定する。*/
            for (int i = 0; i < MAX_BODY_LINES; ++i)
                bodyLines_[i].SetColor(color);
        }


        void PatchNoteContentUI::SetBodyLine(int index, const wchar_t* text)
        {
            /* 指定された行番号の本文のテキストを設定する。*/
            if (index < 0 || index >= MAX_BODY_LINES)
                return;

            /* 本文のテキストを設定する。*/
            bodyLines_[index].SetText(text);
        }


        FontRender& PatchNoteContentUI::GetBodyLineFont(int index)
        {
            /* 指定された行番号の本文の FontRender を取得する。*/
            if (index < 0)
                index = 0;
            if (index >= MAX_BODY_LINES)
                index = MAX_BODY_LINES - 1;

            /* 本文の FontRender を返す。*/
            return bodyLines_[index];
        }


        void PatchNoteContentUI::SplitBodyToLines(const std::wstring& body)
        {
            /* 本文を改行で分割して FontRender に設定する。*/
            for (int i = 0; i < MAX_BODY_LINES; ++i)
                bodyLines_[i].SetText(L"");

            size_t start = 0;
            int lineIndex = 0;

            /* 改行で分割して各行を FontRender に設定する。*/
            while (start <= body.size() && lineIndex < MAX_BODY_LINES)
            {
                /* 改行位置を検索する。*/
                size_t end = body.find(L'\n', start);
                std::wstring line;

                /* 改行が見つからなければ、残りの文字列を取得する。*/
                if (end == std::wstring::npos)
                {
                    line = body.substr(start);
                    start = body.size() + 1;
                }

                /* 改行が見つかれば、改行までの文字列を取得する。*/
                else
                {
                    line = body.substr(start, end - start);
                    start = end + 1;
                }

                /* 空行でなければ、FontRender に設定する。*/
                if (!line.empty())
                {
                    bodyLines_[lineIndex].SetText(line.c_str());
                    ++lineIndex;
                }

                /* 改行が見つからなければ、ループを終了する。*/
                if (end == std::wstring::npos)
                    break;
            }
        }
    } // namespace nsUI
} // namespace nsApp
