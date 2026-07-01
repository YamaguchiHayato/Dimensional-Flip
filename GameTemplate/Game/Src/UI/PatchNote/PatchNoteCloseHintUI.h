#pragma once

#include "Src/UI/Framework/UIImage.h"
#include "Src/UI/Framework/UITransform.h"

/**
 * @file   PatchNoteCloseHintUI.h
 * @brief  パッチノート左下の B ボタンと Back 表記。
 */

namespace nsApp
{
    namespace nsUI
    {
        class PatchNoteCloseHintUI
        {
        public:
            /**
             * @brief 初期化する。
             * @param parent 紙ウィンドウの Transform。Back 文字の座標計算にも使う。
             */
            void Init(UITransform* parent);

            /** @brief 各 UI の Transform / 文字位置を更新する。 */
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


        private:
            UITransform* parent_ = nullptr; //!< 紙ウィンドウ Transform（Back 座標の基準）。

            UIImage button_;         //!< B ボタン画像（ButtonB.DDS）。
            FontRender backText_;    //!< "Back" 文字。
            bool isVisible_ = false; //!< 表示フラグ。
        };
    } // namespace nsUI
} // namespace nsApp
