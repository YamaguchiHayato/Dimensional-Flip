#include "stdafx.h"

#include "Game.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/production/Fade.h"
// UI。
#include "Src/UI/UIBase.h"

// マネージャー。
#include "Src/Core/CameraManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Actor/Stage/StageSetup.h"
#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"

#include "Src/Core/Job/JobQueue.h"
#include "Src/Core/StageLoadContext.h"
#include "Src/Parameter/Stage/StageMasterTable.h"   

static GameSoundList StageToBgm(StageID id)
{
    switch (id)
    {
    case StageID::sStage1:
        return GameSoundList_BGM_Stage1;

    case StageID::sStageEX:
        return GameSoundList_BGM_BOSS;

    default:
        return GameSoundList_BGM_Stage1;
    }
}

namespace nsApp
{
    namespace nsCore
    {
        Game::~Game()
        {
            buildHelper_.DestroyGameplayHud();
            buildHelper_.DestroyPauseMenuUi(); 

            if (pBackGrounds_)
            {
                DeleteGO(pBackGrounds_);
                pBackGrounds_ = nullptr;
            }

            if (pPlayer_)
            {
                DeleteGO(pPlayer_);
                pPlayer_ = nullptr;
            }

            if (pCameraManager_)
            {
                DeleteGO(pCameraManager_);
                pCameraManager_ = nullptr;
            }

            nsApp::nsStage::StageManager::DeleteInstance();

        }


        bool Game::Start()
        {
            // 乱数のシード値を初期化する。
            srand(static_cast<unsigned int>(time(nullptr)));

            // JobQueue を起動する。
            nsApp::nsJob::JobQueue::GetInstance().Startup(2);

            // ステージ情報が必要なため、Build より先に StageManager を生成する。
            nsApp::nsStage::StageManager::CreateInstance();

            const StageID startStageID = nsApp::nsStage::StageManager::GetInstance()->GetNextInitStageID();

            // TSV 読み込みとステージ背景生成（InGameBuildHelper）。
            {
                /* InGameBuildHelperクラスを初期化。*/
                buildHelper_.Initialize(startStageID);

                /* InGameBuildHelperの初期化が完了するまでループ。*/
                while (!buildHelper_.IsFinished())
                    buildHelper_.Update();

                /* 初期化が失敗した場合はデバッグ出力。*/
                if (!buildHelper_.IsLoadSuccess())
                    OutputDebugStringA("Game::Start - parameter TSV load failed.\n");
            }

            // 背景は共通ヘルパーで1本化
            RefreshStageBackGround(startStageID);

            g_renderingEngine->SetStageBackGroundRenderer(
                [this](RenderContext& rc, RenderTarget& mainRT)
                {
                    if (pBackGrounds_ != nullptr)
                        pBackGrounds_->RenderToMainTarget(rc, mainRT);
                });

            // サウンドの生成。
            app::core::SoundManager::CreateInstence();
            app::core::SoundManager::GetInstance()->Init();

            buildHelper_.ConnectGameplayHudData();
            buildHelper_.ConnectBossHudData();
            pauseController_.Initialize(buildHelper_.GetPauseMenuUI());

            if (auto* pHubData = buildHelper_.GetGameplayHudData())
            {
                pHubData->SetTimerSeconds(static_cast<int>(stageTimer_));
                pHubData->SetScore(0);
                pHubData->SetPlayerHpRatio(1.0f);
            }

            // Playerの生成。
            PlayerCreateInstance();

            // CameraManagerの生成。
            pCameraManager_ = NewGO<CameraManager>(0, "cameramanager");
            pPlayer_->InitCameraManager(pCameraManager_);

            // SceneManagerから Fade を取得。
            pFade_ = SceneManager::GetInstance()->GetFade();
            if (pFade_ == nullptr)
                return false;

            // 遷移ステートを初期化。
            state_ = SceneTransitionState::None;
            nextStageID_ = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();

            // Playerの初期位置設定。
            pPlayer_->SetPlayerPos(Vector3(0.0f, 20.0f, 0.0f));

            // 物理デバッグワイヤーフレーム表示有効化。
            PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
            pFade_ = SceneManager::GetInstance()->GetFade();

            return true;
        }


        void Game::Update()
        {
            app::core::SoundManager::GetInstance()->Update();

            nsApp::nsJob::JobQueue::GetInstance().PumpMain();

            StageID stage = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();
            const auto& master = nsApp::nsSystem::StageMasterTable::Get(stage);
            const bool useFormulaBg = (master.backgroundType == "Scroll");


            // フェードイン開始。
            if (!m_isFadeInEnd)
            {
                pFade_->StartFadeIn();

                m_isFadeInEnd = true;
            }

            // 数式背景のパララックス更新。
            float cameraX = g_camera3D->GetTarget().x;

            // 遷移処理を毎フレーム更新。
            UpdateTransition();

            // ポーズ処理を毎フレーム更新。
            const auto pauseAction = pauseController_.Update(this);
            if (pauseController_.IsPaused())
            {
                if (pPlayer_)
                    pPlayer_->SetPaused(true);

                if (pauseAction == nsCore::PauseAction::BackToTitle)
                    SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);

                return;
            }
            else
                if (pPlayer_ && !nsApp::nsStage::StageSetup::ShouldKeepPlayerPaused())
                    pPlayer_->SetPaused(false);


            if (state_ != SceneTransitionState::None)
                return;

            if (!pFade_->IsFadeInEnd())
                return;

            UpdateStageTimer();

            if (auto* pHudData = buildHelper_.GetGameplayHudData())
            {
                int score = 0;
                if (pPlayer_ != nullptr)
                    score = pPlayer_->GetScore();

                float hpRatio = 1.0f;
                if (pPlayer_ != nullptr)
                {
                    const int maxHp = pPlayer_->GetMaxHP();
                    if (maxHp > 0)
                        hpRatio = static_cast<float>(pPlayer_->GetHP()) / maxHp;
                }

                pHudData->SetTimerSeconds(static_cast<int>(stageTimer_));
                pHudData->SetScore(score);
                pHudData->SetPlayerHpRatio(hpRatio);
            }

            if (state_ == SceneTransitionState::None && !m_hasAppliedStageBgm_)
            {
                const StageID stage = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();
                app::core::SoundManager::GetInstance()->PlayBGM(StageToBgm(stage));
                m_hasAppliedStageBgm_ = true;
            }

            // ケースをNoneの状態でのみ、ゲームのメインロジックを実行。
            if (state_ == SceneTransitionState::None)
                nsApp::nsStage::StageManager::GetInstance()->Update();


            // プレイヤーのHPが0以下ならゲームオーバーへ遷移。
            if (pPlayer_ && pPlayer_->GetHP() <= 0)
            {
                SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
                return;
            }
        }


        void Game::RequestStageTransition(StageID nextStageID)
        {
            // 遷移中でなければリクエストを受け付ける。
            if (state_ == SceneTransitionState::None)
            {
                nextStageID_ = nextStageID;
                m_hasAppliedStageBgm_ = false;

                ResetStageTimer();
                // フェードアウト開始。
                state_ = SceneTransitionState::FadeOut;
            }
        }


        void Game::RefreshStageBackGround(StageID stageID)
        {
            // 古い背景 GO を確実に消す（名前 "BackGround"）
            if (auto* oldBg = FindGO<nsApp::nsStage::nsBackGround::IBackGround>("BackGround"))
                DeleteGO(oldBg);

            if (pBackGrounds_ != nullptr)
            {
                DeleteGO(pBackGrounds_);
                pBackGrounds_ = nullptr;
            }

            pBackGrounds_ = buildHelper_.CreateBackGround(stageID);

            if (pBackGrounds_ == nullptr)
                OutputDebugStringA("Game::RefreshStageBackGround - CreateBackGround failed.\n");
        }


        void Game::ChangeDimension(CameraMode mode)
        {
            if (pCameraManager_)
            {
                if (mode == CameraMode::mode2D)
                    pCameraManager_->Request2DMode();
                else
                    pCameraManager_->Request3DMode();
            }

            auto& collisionMgr = nsCollision::CollisionManager::GetInstance();
            if (mode == CameraMode::mode2D)
                collisionMgr.SetDimension(nsCollision::DimensionMode::dim2D);
            else
                collisionMgr.SetDimension(nsCollision::DimensionMode::dim3D);
        }


        void Game::UpdateTransition()
        {
            switch (state_)
            {
            case SceneTransitionState::None:
                break;

            case SceneTransitionState::FadeOut:
            {
                pPlayer_->SetPaused(true);
                pFade_->StartFadeOut();
                stageClearTimer_.Start();
                state_ = SceneTransitionState::Load;
                break;
            }

            case SceneTransitionState::Load:
            {
                stageClearTimer_.Stop();
                if (pFade_->IsFadeOutEnd())
                {
                    SceneManager::GetInstance()->ShowLoading();
                    state_ = SceneTransitionState::Load_Render;
                }
                break;
            }

            case SceneTransitionState::Load_Render:
            {
                nsApp::nsStage::StageLoadContext::Reset();
                stageLoadPhase_ = StageLoadPhase::Idle;
                stageLoadWorkerJobId_ = 0;
                stageLoadMainJobId_ = 0;
                isStageLoadMainEnqueued_ = false;

                state_ = SceneTransitionState::Load_Wait;
                break;
            }

            case SceneTransitionState::Load_Wait:
            {
                const StageID targetStageID = nextStageID_;
                auto& jobQueue = nsApp::nsJob::JobQueue::GetInstance();

                // --- Phase 1: Worker Job を投げる（1回だけ）---
                if (stageLoadPhase_ == StageLoadPhase::Idle)
                {
                    stageLoadWorkerJobId_ = jobQueue.EnqueueWorker([targetStageID]() { nsApp::nsStage::StageLoadContext::PrepareOnWorker(targetStageID); }).GetId();

                    stageLoadPhase_ = StageLoadPhase::Worker;
                    break;
                }

                // --- Phase 2: Worker 完了待ち ---
                if (stageLoadPhase_ == StageLoadPhase::Worker)
                {
                    if (!jobQueue.IsJobDone(stageLoadWorkerJobId_))
                        break;

                    if (!isStageLoadMainEnqueued_)
                    {
                        isStageLoadMainEnqueued_ = true;

                        stageLoadMainJobId_ =
                            jobQueue
                                .EnqueueMain(
                                    [this, targetStageID]()
                                    {
                                        nsApp::nsStage::StageManager::GetInstance()->ChangeStageSync(targetStageID);

                                        RefreshStageBackGround(targetStageID);
                                        const Vector3 newStartPos =
                                            nsApp::nsStage::StageManager::GetInstance()->GetStageStartPos();
                                        pPlayer_->SetPlayerPos(newStartPos);
                                    })
                                .GetId();

                        stageLoadPhase_ = StageLoadPhase::Main;
                    }
                    break;
                }

                // --- Phase 4: Main Job 完了待ち ---
                if (stageLoadPhase_ == StageLoadPhase::Main)
                {
                    if (!jobQueue.IsJobDone(stageLoadMainJobId_))
                        break;

                    stageLoadPhase_ = StageLoadPhase::Idle;
                    isStageLoadMainEnqueued_ = false;
                    state_ = SceneTransitionState::Load_WaitFinish;
                }

                break;
            }
            case SceneTransitionState::Load_WaitFinish:
            {
                state_ = SceneTransitionState::FadeIn;
                break;
            }
            case SceneTransitionState::FadeIn:
            {
                SceneManager::GetInstance()->HideLoading();

                // ボス戦カットイン中は Pause を維持（StageSetup が終了後に解除）
                if (!nsApp::nsStage::StageSetup::ShouldKeepPlayerPaused())
                    pPlayer_->SetPaused(false);

                pFade_->StartFadeIn();
                ResetStageTimer();
                state_ = SceneTransitionState::FadeIn_Wait;
                break;
            }


            case SceneTransitionState::FadeIn_Wait:
            {
                if (pFade_->IsFadeInEnd())
                {
                    nextStageID_ = StageID::sInvalid;
                    state_ = SceneTransitionState::None;
                }
                break;
            }

            default:
                break;
            }
        }


        void Game::ResetStageTimer()
        {
            stageTimer_ = 90.0f;
            timeUpFlag_ = false;

            if (auto* pHudData = buildHelper_.GetGameplayHudData())
               pHudData->SetTimerSeconds(static_cast<int>(stageTimer_));
        }


        void Game::UpdateStageTimer()
        {
            auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
            if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
                return;
            if (timeUpFlag_)
                return;
            if (stageTimer_ > 0.0f)
            {
                stageTimer_ -= g_gameTime->GetFrameDeltaTime();
                if (stageTimer_ <= 0.0f)
                {
                    stageTimer_ = 0.0f;
                    timeUpFlag_ = true;
                    SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
                }
            }
        }


        void Game::PlayerCreateInstance()
        {
            pPlayer_ = NewGO<Player>(1, "player");
        }
    } // namespace core
} // namespace app
