#pragma once

#include <vector>

#include "PatchNoteListItemUI.h"
#include "PatchNoteTypes.h"
#include "Src/UI/Framework/UITransform.h"

/**
 * @file   PatchNoteListUI.h
 * @brief  左ペインのバージョンリスト全体。選択・入力・スクロールを管理する。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PatchNoteListUI
         * @brief Note 左側に並ぶバージョン選択リスト。
         * @details
         *  上下入力で選択を切り替え、選択行が表示範囲外なら scrollOffsetY_ で追従する。
         *  将来の古い ver 追加に備え、最大 MAX_ITEMS 件まで保持する。
         */
        class PatchNoteListUI
        {
        public:
            static const int MAX_ITEMS = 32; //!< 保持できる最大 ver 数。

            /**
             * @brief リスト全体を初期化する。
             * @param parent 親 Transform。nullptr なら画面座標で配置する。
             */
            void Init(UITransform* parent);

            /**
             * @brief 表示する ver 一覧を設定する。
             * @param entries パッチノートエントリ配列。先頭が最新 ver。
             */
            void SetEntries(const std::vector<PatchNoteEntry>& entries);

            /**
             * @brief 各 ListItem の Transform / 文字位置を更新する。
             */
            void Update();

            /**
             * @brief リストを描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief リスト全体の表示・非表示を切り替える。
             * @param visible 表示フラグ。
             */
            void SetVisible(bool visible);

            /**
             * @brief 上下入力を処理する。
             * @param isUp   上入力があったか。
             * @param isDown 下入力があったか。
             * @return 選択が変わったら true。
             */
            bool UpdateInput(bool isUp, bool isDown);

            /**
             * @brief 現在選択中のインデックスを返す。
             * @return 0 始まりのインデックス。
             */
            int GetSelectedIndex() const { return selectedIndex_; }


        private:
            /**
             * @brief 各行の select / unselect 状態を更新する。
             */
            void RefreshSelection();

            /**
             * @brief scrollOffsetY_ を反映して各行の位置と表示可否を更新する。
             */
            void LayoutItems();

            /**
             * @brief 選択行が表示範囲内に収まるようスクロール量を補正する。
             */
            void ClampScrollToSelection();

        private:
            PatchNoteListItemUI items_[MAX_ITEMS]; //!< 各行の UI。
            int itemCount_ = 0;                    //!< 現在の ver 数。
            int selectedIndex_ = 0;                //!< 選択中インデックス。初期値 0 = 最新。
            bool isVisible_ = false;               //!< リスト全体の表示フラグ。
            float scrollOffsetY_ = 0.0f;           //!< リスト縦スクロール量（+で上方向へ送る）。
            UITransform* parent_ = nullptr;        //!< Init 時の親 Transform。
        };
    } // namespace nsUI
} // namespace nsApp
