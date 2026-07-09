#include "stdafx.h"

#include "Game.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/production/Fade.h"

// UI。
#include "Src/Presentation/UI/Screens/SoundSettingScreen.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreenHost.h"
#include "Src/UI/UIBase.h"

// マネージャー。
#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"
#include "Src/Actor/Stage/StageSetup.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Job/JobQueue.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Core/StageLoadContext.h"
#include "Src/Core/StageManager.h"
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
            /* ゲームプレイ HUD を破棄する。 */
            buildHelper_.DestroyGameplayHud();

            /* ポーズメニュー UI を破棄する。 */
            buildHelper_.DestroyPauseMenuUi();

            /* Sound Setting UI を破棄する。 */
            buildHelper_.DestroySoundSettingUi();

            /* 背景 GO を破棄する。 */
            if (pBackGrounds_)
            {
                DeleteGO(pBackGrounds_);
                pBackGrounds_ = nullptr;
            }

            /* プレイヤー GO を破棄する。 */
            if (pPlayer_)
            {
                DeleteGO(pPlayer_);
                pPlayer_ = nullptr;
            }

            /* カメラマネージャ GO を破棄する。 */
            if (pCameraManager_)
            {
                DeleteGO(pCameraManager_);
                pCameraManager_ = nullptr;
            }

            /* StageManager シングルトンを破棄する。 */
            nsApp::nsStage::StageManager::DeleteInstance();
        }


        bool Game::Start()
        {
            /* 乱数のシード値を初期化する。 */
            srand(static_cast<unsigned int>(time(nullptr)));

            /* JobQueue を起動する。 */
            nsApp::nsJob::JobQueue::GetInstance().Startup(2);

            /* ステージ情報が必要なため、Build より先に StageManager を生成する。 */
            nsApp::nsStage::StageManager::CreateInstance();

            const StageID startStageID = nsApp::nsStage::StageManager::GetInstance()->GetNextInitStageID();

            /* TSV 読み込みとステージ背景生成（InGameBuildHelper）。 */
            {
                /* InGameBuildHelper を初期化する。 */
                buildHelper_.Initialize(startStageID);

                /* 初期化が完了するまでループする。 */
                while (!buildHelper_.IsFinished())
                    buildHelper_.Update();

                /* 失敗時はデバッグ出力する。 */
                if (!buildHelper_.IsLoadSuccess())
                    OutputDebugStringA("Game::Start - parameter TSV load failed.\n");
            }

            /* 背景は共通ヘルパーで1本化する。 */
            RefreshStageBackGround(startStageID);

            /* Forward 直前用の背景描画コールバックを登録する。 */
            g_renderingEngine->SetStageBackGroundRenderer(
                [this](RenderContext& rc, RenderTarget& mainRT)
                {
                    if (pBackGrounds_ != nullptr)
                        pBackGrounds_->RenderToMainTarget(rc, mainRT);
                });

            /* サウンドを生成・初期化する。 */
            app::core::SoundManager::CreateInstence();
            app::core::SoundManager::GetInstance()->Init();

            /* ゲームプレイ HUD / ボス HUD を接続する。 */
            buildHelper_.ConnectGameplayHudData();
            buildHelper_.ConnectBossHudData();

            /* ポーズコントローラを初期化する。 */
            pauseController_.Initialize(buildHelper_.GetPauseMenuUI());

            /* Sound Setting を接続し、Controller を初期化する。 */
            buildHelper_.ConnectSoundSettingData();
            nsUI::SoundSettingScreen* pSoundScreen = nullptr;
            if (auto* pHost = buildHelper_.GetSoundSettingScreenHost())
                pSoundScreen = pHost->GetSoundSettingScreen();

            /* PauseMenu / Screen / Data を Setting コントローラへ渡す。 */
            soundSettingController_.Initialize(buildHelper_.GetPauseMenuUI(), pSoundScreen, buildHelper_.GetSoundSettingData());
            /* HUD の初期値を設定する。 */
            if (auto* pHubData = buildHelper_.GetGameplayHudData())
            {
                pHubData->SetTimerSeconds(static_cast<int>(stageTimer_));
                pHubData->SetScore(0);
                pHubData->SetPlayerHpRatio(1.0f);
            }

            /* Player を生成する。 */
            PlayerCreateInstance();

            /* CameraManager を生成し、Player へ渡す。 */
            pCameraManager_ = NewGO<CameraManager>(0, "cameramanager");
            pPlayer_->InitCameraManager(pCameraManager_);

            /* SceneManager から Fade を取得する。 */
            pFade_ = SceneManager::GetInstance()->GetFade();
            if (pFade_ == nullptr)
                return false;

            /* 遷移ステートを初期化する。 */
            state_ = SceneTransitionState::None;
            nextStageID_ = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();

            /* Player の初期位置を設定する。 */
            pPlayer_->SetPlayerPos(Vector3(0.0f, 20.0f, 0.0f));

            /* 物理デバッグワイヤーフレームを有効化する。 */
            PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
            pFade_ = SceneManager::GetInstance()->GetFade();

            return true;
        }


        void Game::Update()
        {
            /* Main スレッド Job を消化する。 */
            nsApp::nsJob::JobQueue::GetInstance().PumpMain();

            StageID stage = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();
            const auto& master = nsApp::nsSystem::StageMasterTable::Get(stage);
            const bool useFormulaBg = (master.backgroundType == "Scroll");

            /* 初回のみフェードインを開始する。 */
            if (!m_isFadeInEnd)
            {
                pFade_->StartFadeIn();
                m_isFadeInEnd = true;
            }

            /* 数式背景のパララックス更新用にカメラ X を取得する。 */
            float cameraX = g_camera3D->GetTarget().x;

            /* 遷移処理を毎フレーム更新する。 */
            UpdateTransition();

            /* ポーズ由来の遷移要求があれば次フレームで実行する。 */
            if (pendingPauseScene_ != nsScene::SceneID::sInvalid)
            {
                /* 時間固定が残っているとロード時間が進まないため解除する。 */
                g_gameTime->DisableFixedFrameDeltaTime();

                if (pPlayer_)
                    pPlayer_->SetPaused(false);

                SceneManager::GetInstance()->ShowLoading();
                SceneManager::GetInstance()->ChangeScene(pendingPauseScene_);
                pendingPauseScene_ = nsScene::SceneID::sInvalid;
                return;
            }

            /* Setting 画面が開いていれば、そちらを優先する（ゲームは再開しない）。 */
            if (soundSettingController_.IsOpen())
            {
                soundSettingController_.Update(this);
                if (pPlayer_)
                    pPlayer_->SetPaused(true);
                return;
            }

            /* ポーズ処理を毎フレーム更新する。 */
            const auto pauseAction = pauseController_.Update(this);
            if (pauseAction == nsCore::PauseAction::RestartStage)
            {
                /* ポーズを閉じるだけ（現在地のまま再開）。 */
                if (pPlayer_)
                    pPlayer_->SetPaused(false);
                return;
            }
            if (pauseAction == nsCore::PauseAction::BackToWorldSelect)
            {
                /* 即時遷移せず、次フレームで安全に遷移する。 */
                pendingPauseScene_ = SceneID::sWorldSelect;
                return;
            }
            if (pauseAction == nsCore::PauseAction::SoundSetting)
            {
                /* ポーズは維持したまま Setting を開く。 */
                soundSettingController_.Open();
                if (pPlayer_)
                    pPlayer_->SetPaused(true);
                return;
            }
            if (pauseController_.IsPaused())
            {
                /* ポーズ中はプレイヤーを止める。 */
                if (pPlayer_)
                    pPlayer_->SetPaused(true);
                return;
            }
            else
            {
                /* ポーズ解除時は、ボスカットイン中以外でプレイヤーを再開する。 */
                if (pPlayer_ && !nsApp::nsStage::StageSetup::ShouldKeepPlayerPaused())
                    pPlayer_->SetPaused(false);
            }

            /* 遷移中は以降のゲーム進行を止める。 */
            if (state_ != SceneTransitionState::None)
                return;

            /* フェードイン完了前は以降の処理を止める。 */
            if (!pFade_->IsFadeInEnd())
                return;

            /* ステージタイマーを更新する。 */
            UpdateStageTimer();

            /* ゲームプレイ HUD へ現在値を反映する。 */
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

            /* 現在ステージの BGM を一度だけ適用する。 */
            if (state_ == SceneTransitionState::None && !m_hasAppliedStageBgm_)
            {
                const StageID stage = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();
                app::core::SoundManager::GetInstance()->PlayBGM(StageToBgm(stage));
                m_hasAppliedStageBgm_ = true;
            }

            /* None のときだけステージ本体を更新する。 */
            if (state_ == SceneTransitionState::None)
                nsApp::nsStage::StageManager::GetInstance()->Update();

            /* プレイヤー HP が 0 以下ならゲームオーバーへ遷移する。 */
            if (pPlayer_ && pPlayer_->GetHP() <= 0)
            {
                SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
                return;
            }
        }


        void Game::RequestStageTransition(StageID nextStageID)
        {
            /* 遷移中でなければリクエストを受け付ける。 */
            if (state_ == SceneTransitionState::None)
            {
                nextStageID_ = nextStageID;
                m_hasAppliedStageBgm_ = false;

                ResetStageTimer();

                /* フェードアウト開始。 */
                state_ = SceneTransitionState::FadeOut;
            }
        }


        void Game::RefreshStageBackGround(StageID stageID)
        {
            /* 古い背景 GO を確実に消す（名前 "BackGround"）。 */
            if (auto* oldBg = FindGO<nsApp::nsStage::nsBackGround::IBackGround>("BackGround"))
                DeleteGO(oldBg);

            if (pBackGrounds_ != nullptr)
            {
                DeleteGO(pBackGrounds_);
                pBackGrounds_ = nullptr;
            }

            /* 新しい背景を生成する。 */
            pBackGrounds_ = buildHelper_.CreateBackGround(stageID);

            if (pBackGrounds_ == nullptr)
                OutputDebugStringA("Game::RefreshStageBackGround - CreateBackGround failed.\n");
        }


        void Game::ChangeDimension(CameraMode mode)
        {
            /* カメラモードを切り替える。 */
            if (pCameraManager_)
            {
                if (mode == CameraMode::mode2D)
                    pCameraManager_->Request2DMode();
                else
                    pCameraManager_->Request3DMode();
            }

            /* コリジョン次元も合わせる。 */
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
                /* プレイヤーを止め、フェードアウトを開始する。 */
                pPlayer_->SetPaused(true);
                pFade_->StartFadeOut();
                stageClearTimer_.Start();
                state_ = SceneTransitionState::Load;
                break;
            }

            case SceneTransitionState::Load:
            {
                /* フェードアウト完了後にローディングを表示する。 */
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
                /* 非同期ロード用の状態をリセットする。 */
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

                /* Phase 1: Worker Job を投げる（1回だけ）。 */
                if (stageLoadPhase_ == StageLoadPhase::Idle)
                {
                    stageLoadWorkerJobId_ =
                        jobQueue
                            .EnqueueWorker([targetStageID]()
                                           { nsApp::nsStage::StageLoadContext::PrepareOnWorker(targetStageID); })
                            .GetId();

                    stageLoadPhase_ = StageLoadPhase::Worker;
                    break;
                }

                /* Phase 2: Worker 完了待ち。 */
                if (stageLoadPhase_ == StageLoadPhase::Worker)
                {
                    if (!jobQueue.IsJobDone(stageLoadWorkerJobId_))
                        break;

                    if (!isStageLoadMainEnqueued_)
                    {
                        isStageLoadMainEnqueued_ = true;

                        /* Phase 3: Main Job でステージ切替と背景再生成を行う。 */
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

                /* Phase 4: Main Job 完了待ち。 */
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
                /* ローディングを隠し、フェードインを開始する。 */
                SceneManager::GetInstance()->HideLoading();

                /* ボス戦カットイン中は Pause を維持する（StageSetup が終了後に解除）。 */
                if (!nsApp::nsStage::StageSetup::ShouldKeepPlayerPaused())
                    pPlayer_->SetPaused(false);

                pFade_->StartFadeIn();
                ResetStageTimer();
                state_ = SceneTransitionState::FadeIn_Wait;
                break;
            }

            case SceneTransitionState::FadeIn_Wait:
            {
                /* フェードイン完了で遷移を終了する。 */
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
            /* 制限時間とタイムアップフラグを初期化する。 */
            stageTimer_ = 90.0f;
            timeUpFlag_ = false;

            /* HUD 表示も合わせてリセットする。 */
            if (auto* pHudData = buildHelper_.GetGameplayHudData())
                pHudData->SetTimerSeconds(static_cast<int>(stageTimer_));
        }


        void Game::UpdateStageTimer()
        {
            /* ボス戦では制限時間を使わない。 */
            auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
            if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
                return;

            /* タイムアップ済みなら何もしない。 */
            if (timeUpFlag_)
                return;

            if (stageTimer_ > 0.0f)
            {
                stageTimer_ -= g_gameTime->GetFrameDeltaTime();
                if (stageTimer_ <= 0.0f)
                {
                    /* 時間切れでゲームオーバーへ遷移する。 */
                    stageTimer_ = 0.0f;
                    timeUpFlag_ = true;
                    SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
                }
            }
        }


        void Game::RestartCurrentStageImmediate()
        {
            auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
            if (pStageManager == nullptr)
                return;

            const StageID current = pStageManager->GetCurrentStageID();

            /* Restart 前の座標を保存する。 */
            Vector3 resumePos = Vector3::Zero;
            if (pPlayer_ != nullptr)
                resumePos = pPlayer_->GetPlayerPos();

            /* 同ステージでも強制的に作り直す。 */
            pStageManager->ChangeStageSync(current, true);

            /* 背景を作り直す。 */
            RefreshStageBackGround(current);

            if (pPlayer_ != nullptr)
            {
                /* 開始位置ではなく、Restart 前の座標へ戻す。 */
                pPlayer_->SetPlayerPos(resumePos);
                pPlayer_->SetRespwanPos(resumePos);
                pPlayer_->ResetForStageRestart();
                pPlayer_->SetPaused(false);
            }

            /* タイマーと BGM 再適用フラグをリセットする。 */
            ResetStageTimer();
            m_hasAppliedStageBgm_ = false;
        }

        void Game::PlayerCreateInstance()
        {
            /* プレイヤー GO を生成する。 */
            pPlayer_ = NewGO<Player>(1, "player");
        }
    } // namespace nsCore
} // namespace nsApp
