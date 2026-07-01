#pragma once

#include "Src/UI/Framework/UIImage.h"

/**
 * @file   PatchNoteWindowUI.h
 * @brief  パッチノートの紙ウィンドウ本体（note.DDS）。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PatchNoteWindowUI
         * @brief 中央に表示する紙 UI の土台。
         */
        class PatchNoteWindowUI
        {
        public:
            /**
             * @brief スプライトを初期化する。
             */
            void Init();

            /**
             * @brief Transform を Sprite に同期する。
             */
            void Update();

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief 表示・非表示を切り替える。
             * @param visible 
             */
            void SetVisible(bool visible);

            /**
             * @brief 紙ウィンドウの Transform を返す。
             * @return UITransform 参照。
             */
            UITransform& GetTransform();


        private:
            UIImage image_; //!< patcNote/note.DDS。
        };
    } // namespace nsUI
} // namespace nsApp
