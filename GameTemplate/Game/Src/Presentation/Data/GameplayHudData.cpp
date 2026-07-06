#include "stdafx.h"

#include "Src/Presentation/Data/GameplayHudData.h"
#include "Src/Presentation/UI/Screens/GameplayHudScreen.h"

namespace nsApp
{
    namespace nsPresentation
    {
        GameplayHudData::GameplayHudData() : pScreen_(nullptr), timerSeconds_(0), score_(0), playerHpRatio_(1.0f) {}


        void GameplayHudData::SetScreen(nsUI::GameplayHudScreen* pScreen)
        {
            pScreen_ = pScreen;
        }


        void GameplayHudData::SetTimerSeconds(int seconds)
        {
            timerSeconds_ = (seconds < 0) ? 0 : seconds;
            SyncToScreen();
        }


        void GameplayHudData::SetScore(int score)
        {
            score_ = (score < 0) ? 0 : score;
            SyncToScreen();
        }


        void GameplayHudData::SetPlayerHpRatio(float ratio)
        {
            if (ratio < 0.0f)
                ratio = 0.0f;
            if (ratio > 1.0f)
                ratio = 1.0f;
            playerHpRatio_ = ratio;
            SyncToScreen();
        }


        int GameplayHudData::GetTimerSeconds() const
        {
            return timerSeconds_;
        }


        int GameplayHudData::GetScore() const
        {
            return score_;
        }


        float GameplayHudData::GetPlayerHpRatio() const
        {
            return playerHpRatio_;
        }


        void GameplayHudData::SyncToScreen()
        {
            if (pScreen_ != nullptr)
                pScreen_->SyncFromDataSource();
        }
    } /* namespace nsPresentation */
} /* namespace nsApp */
