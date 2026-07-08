#pragma once

#include <string>

/**
 * @file   BossHudUiNames.h
 * @brief  ボス HUD 用 UI テクスチャ名 → パス解決。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @brief UI 論理名からテクスチャファイルパスを取得する。
         * @param uiName 論理名（bossIcon, hpBar_flame 等）。
         * @return テクスチャパス。未知の名前は空文字。
         */
        std::string FetchUIName(const char* uiName);

    } // namespace nsUI
} // namespace nsApp
