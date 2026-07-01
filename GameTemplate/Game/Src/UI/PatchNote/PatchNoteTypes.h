#pragma once

#include <string>

/**
 * @file   PatchNoteTypes.h
 * @brief  パッチノート画面で使うデータ型。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @struct PatchNoteEntry
         * @brief 左リスト1件＋右ペイン表示内容。
         */
        struct PatchNoteEntry
        {
            std::wstring version;   //!< 左バー上段（例: L"v2.0.0"）。
            std::wstring listTitle; //!< 左バー下段・短いタイトル（例: L"Namespace"）。12文字以内推奨。
            std::wstring title;     //!< 右ペイン見出し（例: L"Namespace Migration"）。
            std::wstring body;      //!< 右本文。\n で改行。
            bool isNew = false;     //!< true なら NEW バッジ（Note 下）を表示。
        };

    } // namespace nsUI
} // namespace nsApp

using PatchNoteEntry = nsApp::nsUI::PatchNoteEntry;
