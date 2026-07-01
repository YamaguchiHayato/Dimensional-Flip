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
         * @details
         *  背景の上に「ver + タイトル」を横1行で表示する。
         *  例: "v2.0.0  Namespace"
         */
        class PatchNoteListItemUI
        {
        public:
            /**
             * @brief 初期化する。
             * @param parent 親 Transform。nullptr なら画面座標で配置する。
             * @param localPos ローカル座標。
             * @param barScale バーの拡大率。1.0f で等倍。
             */
            void Init(UITransform* parent, const Vector3& localPos, float barScale = 1.0f);

            /**
             * @brief 表示するパッチノートエントリを設定する。
             */
            void Update();

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief 表示・非表示を切り替える。
             * @param visible 表示フラグ。
             */
            void SetVisible(bool visible);

            /**
             * @brief 選択状態を切り替える。
             * @param selected true なら選択状態、false なら非選択状態。
             */
            void SetSelected(bool selected);

            /**
             * @brief 表示するパッチノートエントリを設定する。
             * @param entry 表示するパッチノートエントリ。
             */
            void SetEntry(const PatchNoteEntry& entry);

            /**
             * @brief バーのローカル座標を設定する。
             * @param localPos ローカル座標。
             */
            void SetBarPosition(const Vector3& localPos);


        private:
            /**
             * @brief 選択状態に応じて背景画像を切り替える。
             */
            void RefreshBackground();


        private:
            UIImage background_;  //!< select / unselect 背景。
            FontRender lineFont_; //!< 横1行ラベル（ver + タイトル）。
            float barScale_ = 1.0f;
            bool isSelected_ = false;
            bool isVisible_ = false;
            bool loadedSelectArt_ = false; //!< いま select テクスチャを使っているか
        };
    } // namespace nsUI
} // namespace nsApp
