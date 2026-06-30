#include "stdafx.h"

#include "PlayerTutorialPauseStage.h"
#include "Src/Actor/Character/Player/Component/PlayerStateMachine.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Wall.h"
#include "Src/Core/SoundManager.h"

using nsApp::nsActor::nsCharacter::nsPlayer::EnPlayerState;

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/InputManager.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/Tutorial/TutorialCompleteUI.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"
#include "Src/UI/Tutorial/TutorialSequencer.h"

namespace nsApp
{
    namespace nsState
    {
        static bool isMoveTutorialDone_ = false;

        void PlayerTutorialPauseStage::Enter()
        {
            InputManager::GetInstance()->SetDimensionFlipFlag(true);
            if (pPlayer_ && pPlayer_->GetCameraManager())
                pPlayer_->GetCameraManager()->SetButtonActionControl(true);
            pScoreUI_ = FindGO<ScoreUI>("scoreui");
            if (pScoreUI_)
                pScoreUI_->Deactivate();
            pTutorialSequencer_ = NewGO<TutorialSequencer>(0, "TutorialSequencer");
            pTutorialMessageUI_ = FindGO<TutorialMessageUI>("TutorialMessageUI");
            pTutorialCompleteUI_ = NewGO<TutorialCompleteUI>(0, "TutorialCompleteUI");
            if (pTutorialCompleteUI_ == nullptr)
                pTutorialCompleteUI_ = NewGO<TutorialCompleteUI>(0, "TutorialCompleteUI");
            TutorialTextChange();
            if (pTutorialSequencer_)
                pTutorialSequencer_->RefreshMessageList();
        }


        void PlayerTutorialPauseStage::Update()
        {
            Vector3 stickL = {g_pad[0]->GetLStickXF(), 0.0f, g_pad[0]->GetLStickYF()};
            pPlayer_->SetKeyDirection(stickL);

            float speedRate = 1.0f;

            if (!pPlayer_->GetCharacterController().IsOnGround())
                speedRate = 0.7f;

            pPlayer_->CalculateMovement(stickL);

            JumpUpdatingProcessAtTutorial();
            pPlayer_->ApplyMovement();
            AnimationUpdatingProcess();
            ViewpointChangeUpdatingProcessAtTutorial();

            if (pPlayer_ && pPlayer_->GetCameraManager())
                pPlayer_->GetCameraManager()->Update();
        }

        void PlayerTutorialPauseStage::Exit()
        {
            if (pScoreUI_)
            {
                pScoreUI_->Activate();
                pScoreUI_ = nullptr;
            }
            if (pTutorialSequencer_)
            {
                DeleteGO(pTutorialSequencer_);
                pTutorialSequencer_ = nullptr;
            }
        }

        bool PlayerTutorialPauseStage::RequestID(uint8_t& request)
        {
            if (pTutorialSequencer_ && pTutorialSequencer_->IsAllFinished())
            {
                request = EnPlayerState::enState_Idle;
                return true;
            }

            return false;
        }

        void PlayerTutorialPauseStage::AnimationUpdatingProcess()
        {
            if (!pPlayer_->GetCharacterController().IsOnGround())
            {
                const Vector3& stick = pPlayer_->GetKeyDirection();

                if (stick.x < -0.01f)
                    pPlayer_->SetCurrentIndex(1);
                else if (stick.x > 0.01f)
                    pPlayer_->SetCurrentIndex(5);

                return;
            }

            currentTime_ += g_gameTime->GetFrameDeltaTime();
            if (currentTime_ >= 0.4f)
                currentTime_ = 0.0f;

            const Vector3& stick = pPlayer_->GetKeyDirection();

            if (fabsf(stick.x) > 0.1f || fabsf(stick.z) > 0.1f)
            {
                if (stick.x < 0.0f)
                {
                    if (currentTime_ < 0.1f)
                        pPlayer_->SetCurrentIndex(2);
                    else if (currentTime_ < 0.2f)
                        pPlayer_->SetCurrentIndex(0);
                    else if (currentTime_ < 0.3f)
                        pPlayer_->SetCurrentIndex(3);
                    else
                        pPlayer_->SetCurrentIndex(0);
                }
                else
                {
                    if (currentTime_ < 0.1f)
                        pPlayer_->SetCurrentIndex(6);
                    else if (currentTime_ < 0.2f)
                        pPlayer_->SetCurrentIndex(4);
                    else if (currentTime_ < 0.3f)
                        pPlayer_->SetCurrentIndex(7);
                    else
                        pPlayer_->SetCurrentIndex(4);
                }
            }
            else
            {
                if (stick.x < 0.0f)
                    pPlayer_->SetCurrentIndex(0);
                else
                    pPlayer_->SetCurrentIndex(4);
            }
        }

        void PlayerTutorialPauseStage::JumpUpdatingProcessAtTutorial()
        {
            Vector3& speed = pPlayer_->GetMoveSpeed();
            bool isGround = pPlayer_->GetCharacterController().IsOnGround();

            if (isGround)
            {
                if (speed.y < 0.0f)
                    speed.y = 0.0f;

                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    speed.y = pPlayer_->GetJumpPower();

                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Jump);

                    pPlayer_->SetCurrentIndex(pPlayer_->GetKeyDirection().x < 0 ? 0 : 4);
                }
            }
            else
            {
                if (speed.y > 0.0f)
                {
                    if (!g_pad[0]->IsPress(enButtonA))
                        speed.y -= 8.0f * 2.0f;
                    else
                        speed.y -= 8.0f;
                }
                else
                {
                    speed.y -= 8.0f * 1.2f;
                }

                if (speed.y < -200.0f)
                    speed.y = -200.0f;
            }
        }

        void PlayerTutorialPauseStage::ViewpointChangeUpdatingProcessAtTutorial()
        {
            auto* pWall = FindGO<app::stage::Wall>("wall");

            if (pPlayer_->TryFlipDimension(true))
            {
                if (pWall)
                    pWall->SwitchMode();
            }
        }

        void PlayerTutorialPauseStage::TutorialTextChange()
        {
            if (pTutorialSequencer_)
            {
                if (!isMoveTutorialDone_ && pPlayer_->GetPlayerPos().x < 65.0f)
                {
                    pTutorialSequencer_->AddStep(L"Stick: Move",
                                                 [this]() -> bool
                                                 {
                                                     if (pPlayer_->GetPlayerPos().x > 65.0f)
                                                     {
                                                         isMoveTutorialDone_ = true;
                                                         return true;
                                                     }
                                                     return false;
                                                 });
                }

                pTutorialSequencer_->AddStep(L"Flip View",
                                             [this]() -> bool
                                             {
                                                 if (pPlayer_->GetPlayerPos().x < 160.0f)
                                                 {
                                                     if (pTutorialMessageUI_)
                                                         pTutorialMessageUI_->SetMessage(L"");
                                                     return false;
                                                 }

                                                 if (pTutorialMessageUI_)
                                                     pTutorialMessageUI_->SetMessage(L"Flip View");

                                                 if (pPlayer_->GetCameraManager())
                                                 {
                                                     auto mode = pPlayer_->GetCameraManager()->GetCurrentCameraMode();
                                                     return (mode == CameraMode::mode3D);
                                                 }

                                                 return false;
                                             });

                pTutorialSequencer_->AddStep(L"Free Action",
                                             [this]() -> bool
                                             {
                                                 if (pPlayer_->GetPlayerPos().x > 400.0f)
                                                     return true;

                                                 return false;
                                             });
            }
        }
    } // namespace nsState
} // namespace nsApp
