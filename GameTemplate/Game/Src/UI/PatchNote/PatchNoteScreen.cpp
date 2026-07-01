#include "stdafx.h"
#include "PatchNoteScreen.h"


namespace nsApp
{
    namespace nsUI
    {
        bool PatchNoteScreen::Start()
        {
            return true;
        }
        

        void PatchNoteScreen::Init()
        {
            /* 解像度の設定。*/
            UITransform::SetReferenceResolution(1920.0f, 1080.0f);

            /* 各 UI 部品の初期化。*/
            overlay_.Init();
            window_.Init();

            /* ウィンドウの Transform を取得して、子 UI に渡す。*/
            UITransform* windowTransform = &window_.GetTransform();

            /* 子 UI の初期化。親 Transform を渡すことで、ウィンドウの位置に追従する。*/
            titleBar_.Init(windowTransform);
            list_.Init(nullptr); 
            content_.Init(windowTransform);
            closeHint_.Init(windowTransform);

            /* デフォルトのパッチノートエントリを構築し、リストに設定する。*/
            BuildDefaultEntries();

            /* パッチノート画面を閉じた状態にする。*/
            list_.SetEntries(entries_);

            /* 選択中のエントリに基づいて内容 UI を更新する。*/
            RefreshContentFromSelection();
        }


        void PatchNoteScreen::BuildDefaultEntries()
        {
            entries_.clear();

            //! v2.0.0 — 最新（isNew = true で NEW バッジ用。未実装なら見た目だけ）
            entries_.push_back({L"v2.0.0",
                                L"UI Framework",
                                L"UI Framework",
                                L"* UI redesign\n"
                                L"- Added Transform (parent-child layout)\n"
                                L"- Added UITransform (pivot and anchor)\n"
                                L"- Added UIImage (shared DDS sprite UI)\n"
                                L"- Added UIElement (UI tree node)",
                                true});

            //! v1.5.0 — PatchNote用の画像の用意。
            entries_.push_back({L"v1.5.0", L"UI assets",
                                L"Patch Note Assets",
                                L"* Patch Note graphics\n"
                                L"- Added note.DDS (paper window)\n"
                                L"- Added banner.DDS (title ribbon)\n"
                                L"- Added select and unselect.DDS \n"
                                L"- Added badge.DDS (NEW badge)",
                                false});
            //! v1.0.0 — 初期
            entries_.push_back({L"v1.4.0",
                                L"namespace",
                                L"UI Namespace",
                                L"* Namespace cleanup\n"
                                L"- Moved UI classes under nsApp nsUI\n"
                                L"- Updated UIBase and ingame UI\n"
                                L"- Updated ButtonActionUI HPbarUI NumberUI\n"
                                L"- Updated TimerUI and Game.h",
                                false});
        }


        void PatchNoteScreen::SetEntries(const std::vector<PatchNoteEntry>& entries)
        {
            /* パッチノートのエントリリストを設定する。*/ 
            entries_ = entries;
            list_.SetEntries(entries_);
            RefreshContentFromSelection();
        }


        void PatchNoteScreen::Open()
        {
            /* パッチノート画面を開く。*/
            isOpen_ = true;
            overlay_.SetVisible(true);
            window_.SetVisible(true);
            titleBar_.SetVisible(true);
            list_.SetVisible(true);
            content_.SetVisible(true);
            closeHint_.SetVisible(true);
        }


        void PatchNoteScreen::Close()
        {
            /* パッチノート画面を閉じる。*/
            isOpen_ = false;
            overlay_.SetVisible(false);
            window_.SetVisible(false);
            titleBar_.SetVisible(false);
            list_.SetVisible(false);
            content_.SetVisible(false);
            closeHint_.SetVisible(false);
        }


        void PatchNoteScreen::Update()
        {
            /* パッチノート画面の更新処理。*/
            if (!isOpen_)
                return;

            /* UI の更新。*/
            overlay_.Update();
            window_.Update();
            titleBar_.Update();
            list_.Update();
            content_.Update();
            closeHint_.Update();
        }


        void PatchNoteScreen::Render(RenderContext& rc)
        {
            /* パッチノート画面の描画処理。*/
            if (!isOpen_)
                return;

            /* 描画する順番の設定。*/
            overlay_.Draw(rc);
            list_.Draw(rc);
            window_.Draw(rc);
            content_.Draw(rc);
            titleBar_.Draw(rc);
            closeHint_.Draw(rc);
        }


        bool PatchNoteScreen::UpdateInput(bool isUp, bool isDown)
        {
            /* パッチノート画面の入力処理。*/
            if (!isOpen_)
                return false;

            /* リストの選択を更新し、選択が変わった場合は内容を更新する。*/
            if (list_.UpdateInput(isUp, isDown))
                RefreshContentFromSelection();

            if (g_pad[0]->IsTrigger(enButtonB))
                return true;

            return false;
        }


        void PatchNoteScreen::RefreshContentFromSelection()
        {
            /* 選択中のパッチノートエントリに基づいて内容 UI を更新する。*/
            const int index = list_.GetSelectedIndex();
            if (index < 0 || index >= (int) entries_.size())
                return;

            /* 選択中のエントリの内容を設定する。*/
            content_.SetFromEntry(entries_[(size_t) index]);
        }
    } // namespace nsUI
} // namespace nsApp
