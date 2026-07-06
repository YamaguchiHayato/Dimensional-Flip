#include "stdafx.h"

#include "GameplayHubUiNames.h"

namespace nsApp
{
    namespace nsUI
    {
        namespace
        {
            struct GameplayHudUiEntry
            {
                GameplayHudUiId id;
                const char* name;
            };

            const GameplayHudUiEntry kGameplayHudUiTable[] = {
                {GameplayHudUiId::TimerColon, "timer_colon"},
                {GameplayHudUiId::TimerDigit0, "timer_0"},
                {GameplayHudUiId::TimerDigit1, "timer_1"},
                {GameplayHudUiId::TimerDigit2, "timer_2"},
                {GameplayHudUiId::TimerDigit3, "timer_3"},
                {GameplayHudUiId::TimerDigit4, "timer_4"},
                {GameplayHudUiId::TimerDigit5, "timer_5"},
                {GameplayHudUiId::TimerDigit6, "timer_6"},
                {GameplayHudUiId::TimerDigit7, "timer_7"},
                {GameplayHudUiId::TimerDigit8, "timer_8"},
                {GameplayHudUiId::TimerDigit9, "timer_9"},

                {GameplayHudUiId::ScoreDigit0, "score_0"},
                {GameplayHudUiId::ScoreDigit1, "score_1"},
                {GameplayHudUiId::ScoreDigit2, "score_2"},
                {GameplayHudUiId::ScoreDigit3, "score_3"},
                {GameplayHudUiId::ScoreDigit4, "score_4"},
                {GameplayHudUiId::ScoreDigit5, "score_5"},
                {GameplayHudUiId::ScoreDigit6, "score_6"},
                {GameplayHudUiId::ScoreDigit7, "score_7"},
                {GameplayHudUiId::ScoreDigit8, "score_8"},
                {GameplayHudUiId::ScoreDigit9, "score_9"},

                {GameplayHudUiId::PlayerHpBarBack, "hp_bar_back"},
                {GameplayHudUiId::PlayerHpBarFill, "hp_bar_fill"},
            };
        }


        const char* FetchUIName(GameplayHudUiId id)
        {
            /* UIの数を数える。*/
            const size_t count = sizeof(kGameplayHudUiTable) / sizeof(kGameplayHudUiTable[0]);
            for (size_t i = 0; i < count; ++i)
            {
                /* UIの数とテーブルの数が一致する場合。*/
                if (kGameplayHudUiTable[i].id == id)
                    return kGameplayHudUiTable[i].name;
            }
            return "";
        }
    } // namespace nsUI
} // namespace nsApp
