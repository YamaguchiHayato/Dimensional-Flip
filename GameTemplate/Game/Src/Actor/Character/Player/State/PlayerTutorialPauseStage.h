#pragma once

#include "Src/Actor/Character/IState.h"
#include "Src/Actor/Character/PlayerForward.h"

/**
 * @file   PlayerTutorialPauseStage.h
 * @brief  チュートリアル専用のプレイヤーステート。
 */

namespace nsApp
{
    namespace nsUI
    {
        class ScoreUI;
        class TutorialMessageUI;
        class TutorialSequencer;
        class TutorialCompleteUI;
    } // namespace nsUI

    namespace nsState
    {
        /**
         * @class PlayerTutorialPauseStage
         * @brief チュートリアル中の移動・ジャンプ・視点切替を行い、UI シーケンサーと連携する。
         */
        class PlayerTutorialPauseStage : public IState
        {
        public:
            explicit PlayerTutorialPauseStage(Player* pPlayer) : pPlayer_(pPlayer) {}
            virtual ~PlayerTutorialPauseStage() = default;

            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;

        private:
            void AnimationUpdatingProcess();
            void JumpUpdatingProcessAtTutorial();
            void ViewpointChangeUpdatingProcessAtTutorial();
            void TutorialTextChange();

        private:
            Player* pPlayer_ = nullptr;
            nsUI::TutorialMessageUI* pTutorialMessageUI_ = nullptr;
            nsUI::TutorialSequencer* pTutorialSequencer_ = nullptr;
            nsUI::TutorialCompleteUI* pTutorialCompleteUI_ = nullptr;
            nsUI::ScoreUI* pScoreUI_ = nullptr; //!< チュートリアル中のみ非表示。

        private:
            bool isButtonPressed_ = false;
            bool isVisible_ = false;
            float currentTime_ = 0.0f;
            float tutorialTimer_ = 0.0f;
        };
    } // namespace nsState
} // namespace nsApp
