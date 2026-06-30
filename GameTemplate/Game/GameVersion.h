#pragma once

namespace nsApp
{
    namespace nsCore
    {
        /**
         * @brief ゲームのバージョン情報を管理する構造体。
         */
        struct GameVersion
        {
            static constexpr int Major = 0;
            static constexpr int Minor = 1;
            static constexpr int Patch = 0;

            static const wchar_t* GetDisplayString()
            {
                static const wchar_t s_display[] = L"Ver 0.1.0";
                return s_display;
            }
        };
    } // namespace nsCore
}
