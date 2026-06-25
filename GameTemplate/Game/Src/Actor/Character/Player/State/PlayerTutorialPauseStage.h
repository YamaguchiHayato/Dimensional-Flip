#pragma once
#include "Src/Actor/Character/IState.h"
#include "Src/Actor/Character/PlayerForward.h"

// 前方宣言
namespace app
{
    namespace nsUI
    {
        class TutorialMessageUI;
        class TutorialSequencer;
        class TutorialCompleteUI;   
    } 
} 


namespace nsApp
{
    namespace nsState
    {

        class PlayerTutorialPauseStage : public IState
        {
        public:
            PlayerTutorialPauseStage(Player* pPlayer) : pPlayer_(pPlayer) {};
            virtual ~PlayerTutorialPauseStage() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        ////////////////////////////////////
        // チュートリアル時にの動作更新処理。
        ////////////////////////////////////
        private:
            // アニメーションの更新処理。
            void AnimationUpdatingProcess();

            // ジャンプの更新処理。
            void JumpUpdatingProcessAtTutorial();

            // 視点切替処理。
            void ViewpointChangeUpdatingProcessAtTutorial();

            // テキストの切り替え処理。
            void TutorialTextChange();

         ////////////////////////////////////
         ////////////////////////////////////
        /////////////////////////////////////


        private:
            Player* pPlayer_ = nullptr;
            app::nsUI::TutorialMessageUI* pTutorialMessageUI_ = nullptr;
            app::nsUI::TutorialSequencer* pTutorialSequencer_ = nullptr;
            app::nsUI::TutorialCompleteUI* pTutorialCompleteUI_ = nullptr;

        private:
            bool isButtonPressed_ = false;
            bool isVisible_ = false;

            float currentTime_ = 0.0f;
            float tutorialTimer_ = 0.0f;
        };
    } 
} 
