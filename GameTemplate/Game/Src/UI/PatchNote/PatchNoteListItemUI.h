#pragma once

#include "PatchNoteTypes.h"
#include "Src/UI/Framework/UIImage.h"

/**
 * @file   PatchNoteListItemUI.h
 * @brief  バージョンリストの1行（select/unselect 背景＋横書きラベル）。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PatchNoteListItemUI
         * @brief 左ペインのバージョン選択バー1件分を表示する。
         */
        class PatchNoteListItemUI
        {
        public:
            /**
             * @brief PatchNoteListItemUI を初期化する。
             * @param parent 親 Transform。nullptr の場合は画面座標。
             * @param localPos ローカル座標。親 Transform からの相対位置。
             * @param barScale バーの拡大率。1.0f で等倍。
             */
            void Init(UITransform* parent, const Vector3& localPos, float barScale = 1.0f);

            /**
             * @brief PatchNoteListItemUI を更新する。
             */
            void Update();

            /**
             * @brief PatchNoteListItemUI を描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief PatchNoteListItemUI の表示・非表示を切り替える。
             * @param visible 表示フラグ。
             */
            void SetVisible(bool visible);

            /**
             * @brief PatchNoteListItemUI の選択状態を切り替える。
             * @param selected true なら選択状態、false なら非選択状態。
             */
            void SetSelected(bool selected);

            /**
             * @brief PatchNoteListItemUI の表示内容を設定する。
             * @param entry 表示する PatchNoteEntry。
             */
            void SetEntry(const PatchNoteEntry& entry);

            /**
             * @brief PatchNoteListItemUI の位置を設定する。
             * @param localPos ローカル座標。親 Transform からの相対位置。
             */
            void SetBarPosition(const Vector3& localPos);


        private:
            /**
             * @brief PatchNoteListItemUI の背景画像の表示状態を更新する。
             */
            void ApplyBackgroundVisibility();


        private:
            UIImage selectBackground_;   //!< オレンジ枠
            UIImage unselectBackground_; //!< グレー枠
            FontRender lineFont_;        //!< バージョン文字列
            float barScale_ = 1.0f;      //!< バーの拡大率
            bool isSelected_ = false;    //!< 選択状態フラグ
            bool isVisible_ = false;     //< 表示状態フラグ
        };
    } // namespace nsUI
} // namespace nsApp
