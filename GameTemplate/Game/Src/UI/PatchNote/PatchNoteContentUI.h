#pragma once

#include <string>
#include <vector>

#include "PatchNoteTypes.h"
#include "Src/UI/Framework/UITransform.h"

/**
 * @file   PatchNoteContentUI.h
 * @brief  右ペインのテキスト表示。FontRender カスタム用。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PatchNoteContentUI
         * @brief 修正内容（見出し＋本文）を FontRender で描画する。
         */
        class PatchNoteContentUI
        {
        public:
            static const int MAX_BODY_LINES = 14;

            /**
             * @brief 初期化する。
             * @param parent 紙ウィンドウの Transform。文字位置の親。
             */
            void Init(UITransform* parent);

            /**
             * @brief Transform を FontRender に同期する。
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
             * @brief PatchNoteEntry から内容を設定する。
             * @param entry エントリー。
             */
            void SetFromEntry(const PatchNoteEntry& entry);

            /**
             * @brief ヘッダー文字列を設定する。
             * @param text ヘッダー文字列（UTF-16）。
             */
            void SetHeaderText(const wchar_t* text);

            /**
             * @brief 本文文字列を設定する。改行は \n で分割される。
             * @param text 本文文字列（UTF-16）。
             */
            void SetBodyText(const wchar_t* text);

            /**
             * @brief ヘッダー文字のスケールを設定する。
             * @param scale スケール値。
             */
            void SetHeaderScale(float scale);

            /**
             * @brief 本文文字のスケールを設定する。
             * @param scale スケール値。
             */
            void SetBodyScale(float scale);

            /**
             * @brief ヘッダー文字の色を設定する。
             * @param color 色（RGBA）。
             */
            void SetHeaderColor(const Vector4& color);

            /**
             * @brief 本文文字の色を設定する。
             * @param color 色（RGBA）。
             */
            void SetBodyColor(const Vector4& color);

            /**
             * @brief 本文の1行を設定する。
             * @param index 行番号（0～MAX_BODY_LINES-1）。
             * @param text 行文字列（UTF-16）。
             */
            void SetBodyLine(int index, const wchar_t* text);

            /**
             * @brief ヘッダー文字の FontRender を取得する。
             * @return ヘッダー文字の FontRender 参照。
             */
            FontRender& GetHeaderFont() { return headerFont_; }

            /**
             * @brief 本文の FontRender を取得する。
             * @param index 行番号（0～MAX_BODY_LINES-1）。
             * @return 本文の FontRender 参照。
             */
            FontRender& GetBodyLineFont(int index);


        private:
            /**
             * @brief 本文文字列を改行で分割して FontRender に設定する。
             * @param body 本文文字列（UTF-16）。\n で改行。
             */
            void SplitBodyToLines(const std::wstring& body);

            /**
             * @brief ヘッダーと本文の FontRender にデフォルトスタイルを適用する。
             */
            void ApplyDefaultStyle();

            /**
             * @brief ヘッダーと本文の FontRender にワールド座標を適用する。
             */
            void ApplyWorldPositions();


        private:
            UITransform* parent_ = nullptr;          //! < 紙ウィンドウの Transform。
            Vector3 headerLocalPos_ = Vector3::Zero; //!< 紙から見たヘッダー位置。
            float bodyLocalX_ = 120.0f;              //!< 本文 X（紙ローカル）。
            float bodyLocalBaseY_ = 140.0f;          //!< 本文1行目 Y。
            float bodyLineStepY_ = 40.0f;            //!< 行間。
            FontRender headerFont_;                  //!< ヘッダー文字の FontRender。
            FontRender bodyLines_[MAX_BODY_LINES];   //! < 本文文字の FontRender 配列。
            bool isVisible_ = false;                 //!< 表示フラグ。
        };
    } // namespace nsUI
} // namespace nsApp
