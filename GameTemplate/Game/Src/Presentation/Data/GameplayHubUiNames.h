#pragma once

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum GameplayHudUiId
         * @brief インゲーム HUD 用 UI 名 ID。
         */
        enum class GameplayHudUiId : uint8_t
        {
            TimerColon,
            TimerDigit0,
            TimerDigit1,
            TimerDigit2,
            TimerDigit3,
            TimerDigit4,
            TimerDigit5,
            TimerDigit6,
            TimerDigit7,
            TimerDigit8,
            TimerDigit9,

            ScoreDigit0,
            ScoreDigit1,
            ScoreDigit2,
            ScoreDigit3,
            ScoreDigit4,
            ScoreDigit5,
            ScoreDigit6,
            ScoreDigit7,
            ScoreDigit8,
            ScoreDigit9,

            PlayerHpBarBack,
            PlayerHpBarFill,

            Num
        };

        /**
         * @brief UI 名 ID からリソース名を取得する。
         */
        const char* FetchUIName(GameplayHudUiId id);
    } // namespace nsUI
} // namespace nsApp
