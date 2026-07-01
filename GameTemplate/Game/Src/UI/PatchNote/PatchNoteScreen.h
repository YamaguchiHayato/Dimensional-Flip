#pragma once

#include <vector>

#include "PatchNoteCloseHintUI.h"
#include "PatchNoteContentUI.h"
#include "PatchNoteListUI.h"
#include "PatchNoteOverlayerUI.h"
#include "PatchNoteTitleBarUI.h"
#include "PatchNoteTypes.h"
#include "PatchNoteWindowUI.h"

/**
 * @file   PatchNoteScreen.h
 * @brief  パッチノート画面の合成ルート（IGameObject）。
 * @details
 *  各 UI 部品を束ねるだけ。描画ロジックは子クラスに委譲する。
 */

namespace nsApp
{
    namespace nsUI
    {
        class PatchNoteScreen : public IGameObject
        {
        public:
            /**
             * @brief ゲームオブジェクトの初期化。
             * @return 初期化成功なら true。
             */
            bool Start() override;

            /**
             * @brief ゲームオブジェクトの更新。
             */
            void Update() override;

            /**
             * @brief ゲームオブジェクトの描画。
             * @param rc 描画コンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief ゲームオブジェクトの終了。
             */
            void Init();

            /**
             * @brief パッチノート画面を開く。
             */
            void Open();

            /**
             * @brief パッチノート画面を閉じる。
             */
            void Close();

            /**
             * @brief パッチノート画面が開いているか。
             * @return 開いているなら true。
             */
            bool IsOpen() const { return isOpen_; }

            /**
             * @brief 入力処理。
             * @return B で閉じる要求なら true。
             */
            bool UpdateInput(bool isUp, bool isDown);

            /**
             * @brief パッチノートの内容 UI を取得する。
             * @return パッチノートの内容 UI。
             */
            PatchNoteContentUI& GetContentUI() { return content_; }

            /**
             * @brief パッチノートのリスト UI を取得する。
             * @param entries パッチノートのリスト UI。
             */
            void SetEntries(const std::vector<PatchNoteEntry>& entries);


        private:
            /**
             * @brief パッチノートのデフォルトエントリを構築する。
             */
            void BuildDefaultEntries();

            /**
             * @brief パッチノートの内容 UI を選択中のエントリに更新する。
             */
            void RefreshContentFromSelection();


        private:
            PatchNoteOverlayUI   overlay_;   //!< 暗転
            PatchNoteWindowUI    window_;    //!< 紙（note.DDS）
            PatchNoteTitleBarUI  titleBar_;  //!< リボン＋タイトル
            PatchNoteListUI      list_;      //!< 左 ver 選択
            PatchNoteContentUI   content_;   //!< 右 修正内容
            PatchNoteCloseHintUI closeHint_; //!< B + Back

            std::vector<PatchNoteEntry> entries_; //! < パッチノートのエントリリスト
            bool isOpen_ = false;                 //! < パッチノート画面が開いているか]
            int lastContentIndex_ = -1; //!< 本文の二重更新防止
        };
    } // namespace nsUI
} // namespace nsApp

using PatchNoteScreen = nsApp::nsUI::PatchNoteScreen;
