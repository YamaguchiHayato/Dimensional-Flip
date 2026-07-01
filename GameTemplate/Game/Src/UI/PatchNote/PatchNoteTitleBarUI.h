#pragma once

#include "Src/UI/Framework/UIImage.h"
#include "Src/UI/Framework/UITransform.h"

/**
 * @file   PatchNoteTitleBarUI.h
 * @brief  上部リボンと「パッチノート」タイトル文字。
 */

namespace nsApp
{
    namespace nsUI
    {
        class PatchNoteTitleBarUI
        {
        public:
            /**
             * @brief 初期化する。
             * @param parent 紙ウィンドウの Transform（子として追従）。
             */
            void Init(UITransform* parent);

            /**
             * @brief リボンとタイトル文字の Transform を更新する。
             */
            void Update();

            /**
             * @brief リボンとタイトル文字を描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);

            /**
             * @brief リボンとタイトル文字の表示・非表示を切り替える。
             * @param visible 表示フラグ。
             */
            void SetVisible(bool visible);

            /**
             * @brief タイトル文字を設定する。
             * @param text タイトル文字列（L"Patch Note" など）。
             */
            void SetTitleText(const wchar_t* text);


        private:
            UIImage banner_; //! 青色リボン画像。
            FontRender titleFont_; //! タイトル文字描画。
            bool isVisible_ = false; //! 表示フラグ。
        };
    } // namespace nsUI
} // namespace nsApp
