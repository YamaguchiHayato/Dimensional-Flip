#include "stdafx.h"

#include "PlayerTutorialPauseStage.h"
#include "Src/Actor/Character/Player/Component/PlayerStateMachine.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Wall.h"
#include "Src/Core/SoundManager.h"

using nsApp::nsActor::nsCharacter::nsPlayer::EnPlayerState;

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/Game.h"
#include "Src/Core/InputManager.h"
#include "Src/Presentation/UI/Screens/GameplayHudScreen.h"
#include "Src/Presentation/UI/Screens/GameplayHudScreenHost.h"
#include "Src/UI/Tutorial/TutorialCompleteUI.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"
#include "Src/UI/Tutorial/TutorialSequencer.h"

namespace nsApp
{
    namespace nsState
    {
        static bool isMoveTutorialDone_ = false;

        /**
         * @brief コンストラクタ。
         * @param pPlayer 所属するプレイヤーへのポインタ。
         */
        PlayerTutorialPauseStage::PlayerTutorialPauseStage(nsApp::nsActor::nsCharacter::nsPlayer::Player* pPlayer)
            : pPlayer_(pPlayer)
        {
        }

        /**
         * @brief デストラクタ。
         */
        PlayerTutorialPauseStage::~PlayerTutorialPauseStage() = default;


        void PlayerTutorialPauseStage::Enter()
        {
            /* 入力管理クラスから入力を貰う。*/
            InputManager::GetInstance()->SetDimensionFlipFlag(true);

            if (pPlayer_ && pPlayer_->GetCameraManager())
                pPlayer_->GetCameraManager()->SetButtonActionControl(true);

            /**
             * @brief 旧 ScoreUI::Deactivate 相当。
             *        チュートリアル中はスコアだけ非表示（Timer / HP は表示したまま）。
             */
            pGameplayHudHost_ = FindGO<nsUI::GameplayHudScreenHost>("GameplayHudScreenHost");
            if (pGameplayHudHost_)
            {
                if (auto* pScreen = pGameplayHudHost_->GetGameplayHudScreen())
                    pScreen->SetScoreVisible(false);
            }

            /* TutorialUIクラスを生成する。*/
            pTutorialSequencer_ = NewGO<TutorialSequencer>(0, "TutorialSequencer");
            pTutorialMessageUI_ = FindGO<TutorialMessageUI>("TutorialMessageUI");
            pTutorialCompleteUI_ = NewGO<TutorialCompleteUI>(0, "TutorialCompleteUI");

            /* TutorialUIクラスのメッセージを更新する。*/
            if (pTutorialCompleteUI_ == nullptr)
                pTutorialCompleteUI_ = NewGO<TutorialCompleteUI>(0, "TutorialCompleteUI");

            /* TutorialUIクラスのメッセージを更新する。*/
            TutorialTextChange();
            if (pTutorialSequencer_)
                pTutorialSequencer_->RefreshMessageList();
        }


        void PlayerTutorialPauseStage::Update()
        {
            /* 入力管理クラスから入力を貰う。*/
            tutorialStick_ = {g_pad[0]->GetLStickXF(), 0.0f, g_pad[0]->GetLStickYF()};
            pPlayer_->SetKeyDirection(tutorialStick_);

            /* 移動速度を調整する。*/
            if (!pPlayer_->GetCharacterController().IsOnGround())
                speedRate_ = 0.7f;

            /* 移動処理を行う。*/
            pPlayer_->CalculateMovement(tutorialStick_);

            /* ジャンプ処理を行う。*/
            JumpUpdatingProcessAtTutorial();

            /* 移動処理を適用する。*/
            pPlayer_->ApplyMovement();

            /* アニメーション更新処理を行う。*/
            AnimationUpdatingProcess();

            /* 視点切替更新処理を行う。*/
            ViewpointChangeUpdatingProcessAtTutorial();

            /* チュートリアルシーケンサーの進行を更新する。*/
            if (pTutorialSequencer_)
                pTutorialSequencer_->Update();

            /* チュートリアルメッセージの更新処理を行う。*/
            if (pPlayer_ && pPlayer_->GetCameraManager())
                pPlayer_->GetCameraManager()->Update();
        }


        void PlayerTutorialPauseStage::Exit()
        {
            /**
             * @brief 旧 ScoreUI::Activate 相当。
             *        チュートリアル終了後にスコア表示を戻す。
             */
            if (pPlayer_ != nullptr)
                pPlayer_->SetTutorialDone(true);

            nsUI::GameplayHudScreenHost* pHost = pGameplayHudHost_;
            if (pHost == nullptr)
                pHost = FindGO<nsUI::GameplayHudScreenHost>("GameplayHudScreenHost");

            if (pHost != nullptr)
            {
                if (auto* pScreen = pHost->GetGameplayHudScreen())
                    pScreen->SetScoreVisible(true);
            }
            pGameplayHudHost_ = nullptr;

            /* TutorialUIクラスを削除する。*/
            if (pTutorialSequencer_)
            {
                DeleteGO(pTutorialSequencer_);
                pTutorialSequencer_ = nullptr;
            }
        }


        bool PlayerTutorialPauseStage::RequestID(uint8_t& request)
        {
            /* チュートリアルがすべて終了した場合、ステートを Idle に変更する。*/
            if (pTutorialSequencer_ && pTutorialSequencer_->IsAllFinished())
            {
                request = EnPlayerState::enState_Idle;
                return true;
            }

            return false;
        }


        void PlayerTutorialPauseStage::AnimationUpdatingProcess()
        {
            /* プレイヤーのアニメーションを更新する。*/
            if (!pPlayer_->GetCharacterController().IsOnGround())
            {
                /* 空中にいる場合は、ジャンプアニメーションを再生する。*/
                const Vector3& stick = pPlayer_->GetKeyDirection();

                /* 左右の入力に応じて、ジャンプ中のアニメーションを切り替える。*/
                if (stick.x < -0.01f)
                    pPlayer_->SetCurrentIndex(1);

                /* 左右の入力に応じて、ジャンプ中のアニメーションを切り替える。*/
                else if (stick.x > 0.01f)
                    pPlayer_->SetCurrentIndex(5);

                return;
            }

            /* 地上にいる場合は、移動アニメーションを再生する。*/
            currentTime_ += g_gameTime->GetFrameDeltaTime();
            if (currentTime_ >= 0.4f)
                currentTime_ = 0.0f;

            /* 左右の入力に応じて、移動中のアニメーションを切り替える。*/
            const Vector3& stick = pPlayer_->GetKeyDirection();

            /* 左右の入力に応じて、移動中のアニメーションを切り替える。*/
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
            /* ジャンプ処理を行う。*/
            Vector3& speed = pPlayer_->GetMoveSpeed();
            isGround_ = pPlayer_->GetCharacterController().IsOnGround();

            /* ジャンプボタンが押された場合、ジャンプ処理を行う。*/
            if (isGround_)
            {
                /* 地上にいる場合は、ジャンプ処理を行う。*/
                if (speed.y < 0.0f)
                    speed.y = 0.0f;

                /* ジャンプボタンが押された場合、ジャンプ処理を行う。*/
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    speed.y = pPlayer_->GetJumpPower();
                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Jump);
                    pPlayer_->SetCurrentIndex(pPlayer_->GetKeyDirection().x < 0 ? 0 : 4);
                }
            }
            else
            {
                /* 空中にいる場合は、落下処理を行う。*/
                if (speed.y > 0.0f)
                {
                    /* 上昇中の場合は、ジャンプボタンが押されているかどうかで落下速度を調整する。*/
                    if (!g_pad[0]->IsPress(enButtonA))
                        speed.y -= 8.0f * 2.0f;
                    else
                        speed.y -= 8.0f;
                }

                /* 落下中の場合は、落下速度を調整する。*/
                else
                    speed.y -= 8.0f * 1.2f;

                /* 落下速度の上限を設定する。*/
                if (speed.y < -200.0f)
                    speed.y = -200.0f;
            }
        }


        void PlayerTutorialPauseStage::ViewpointChangeUpdatingProcessAtTutorial()
        {
            /* 視点切替処理を行う。*/
            auto* pWall = FindGO<app::stage::Wall>("wall");

            /* 視点切替ボタンが押された場合、視点切替処理を行う。*/
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
