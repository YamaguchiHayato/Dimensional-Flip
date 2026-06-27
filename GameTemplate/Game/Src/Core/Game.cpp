#include "stdafx.h"

#include "Game.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/production/Fade.h"
// UI。
#include "Src/UI/HPbarUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/TimerUI.h"
#include "Src/UI/UIBase.h"

// マネージャー。
#include "Src/Core/CameraManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"

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

namespace app
{
    namespace core
    {
        Game::~Game()
        {
            // ステージ背景を削除。
            if (pBackGrounds_)
            {
                DeleteGO(pBackGrounds_);
                pBackGrounds_ = nullptr;
            }

            // Playerを削除。
            if (pPlayer_)
            {
                DeleteGO(pPlayer_);
                pPlayer_ = nullptr;
            }

            // CameraManagerを削除。
            if (pCameraManager_)
            {
                DeleteGO(pCameraManager_);
                pCameraManager_ = nullptr;
            }

            // 制限時間UIを削除。
            if (pTimerUI_)
                DeleteGO(pTimerUI_);
            if (pNumberUI_)
                DeleteGO(pNumberUI_);

            // スコアUIを削除。
            if (pScoreUI_)
                DeleteGO(pScoreUI_);

            // HPバーUIを削除。
            if (pHpbarUI_)
                DeleteGO(pHpbarUI_);

            // ステージを削除。
            StageManager::DeleteInstance();
            // 音を削除。
            SoundManager::DeleteInstence();
        }


        bool Game::Start()
        {
            // 乱数のシード値を初期化する。
            srand(static_cast<unsigned int>(time(nullptr)));

            // ステージ情報が必要なため、Build より先に StageManager を生成する。
            StageManager::CreateInstance();

            // TSV 読み込みとステージ背景生成（InGameBuildHelper）。
            {
                const StageID startStageID = StageManager::GetInstance()->GetCurrentStageID();
                buildHelper_.Initialize(startStageID);

                while (!buildHelper_.IsFinished())
                    buildHelper_.Update();

                if (!buildHelper_.IsLoadSuccess())
                    OutputDebugStringA("Game::Start - parameter TSV load failed.\n");

                // Build ステップで生成済みの背景を受け取る。
                pBackGrounds_ = buildHelper_.GetBackGround();
            }

            g_renderingEngine->SetStageBackGroundRenderer(
                [](RenderContext& rc, RenderTarget& mainRT)
                {
                    auto* pBG = FindGO<nsApp::nsStage::nsScrollBackGround::ScrollStageBackGround>("Normal");
                    if (pBG != nullptr)
                        pBG->RenderToMainTarget(rc, mainRT);
                });

            // サウンドの生成。
            app::core::SoundManager::CreateInstence();
            app::core::SoundManager::GetInstance()->Init();

            // UIの生成。
            UICreateInstance();

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
            nextStageID_ = StageManager::GetInstance()->GetCurrentStageID();

            // Playerの初期位置設定。
            pPlayer_->SetPlayerPos(Vector3(0.0f, 20.0f, 0.0f));

            // 物理デバッグワイヤーフレーム表示有効化。
            PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
            pFade_ = SceneManager::GetInstance()->GetFade();

            return true;
        }

        void Game::Update()
        {
            StageID stage = StageManager::GetInstance()->GetCurrentStageID();
            const bool useFormulaBg = (stage != StageID::sStageEX);
            g_renderingEngine->EnableCompositeBackground(useFormulaBg);

            // フェードイン開始。
            if (!m_isFadeInEnd)
            {
                pFade_->StartFadeIn();

                m_isFadeInEnd = true;
            }

            // 数式背景のパララックス更新。
            float cameraX = g_camera3D->GetTarget().x;
            g_renderingEngine->UpdateCompositeBackground(cameraX);

            // 遷移処理を毎フレーム更新。
            UpdateTransition();

            if (state_ != SceneTransitionState::None)
                return;

            if (!pFade_->IsFadeInEnd())
                return;

            if (state_ == SceneTransitionState::None && !m_hasAppliedStageBgm_)
            {
                const StageID stage = StageManager::GetInstance()->GetCurrentStageID();
                app::core::SoundManager::GetInstance()->PlayBGM(StageToBgm(stage));
                m_hasAppliedStageBgm_ = true;
            }

            // ケースをNoneの状態でのみ、ゲームのメインロジックを実行。
            if (state_ == SceneTransitionState::None)
            {
                // ステージマネージャーの更新。
                StageManager::GetInstance()->Update();
            }

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

                if (pNumberUI_)
                    pNumberUI_->ResetTimer();

                // フェードアウト開始。
                state_ = SceneTransitionState::FadeOut;
            }
        }

        void Game::ChangeDimension(CameraMode mode)
        {
            // カメラの切り替え。
            if (pCameraManager_)
            {
                if (mode == CameraMode::mode2D)
                    pCameraManager_->Request2DMode();

                else
                    pCameraManager_->Request3DMode();
            }

            // コリジョンの切り替え。
            auto& collision = app::collision::CollisionManager::GetInstance();
            if (mode == CameraMode::mode2D)
                collision.SetDimension(collision::DimensionMode::dim2D);

            else
                collision.SetDimension(collision::DimensionMode::dim3D);
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
                state_ = SceneTransitionState::Load_Wait;
                break;
            }

            case SceneTransitionState::Load_Wait:
            {
                StageManager::GetInstance()->ChangeStageSync(nextStageID_);

                // 旧背景を破棄してから、新ステージ用に作り直す。
                if (pBackGrounds_)
                {
                    DeleteGO(pBackGrounds_);
                    pBackGrounds_ = nullptr;
                }

                const StageID currentStageID = StageManager::GetInstance()->GetCurrentStageID();
                pBackGrounds_ = buildHelper_.CreateBackGround(currentStageID);

                // プレイヤーの位置を新しいステージの開始位置にリセット。
                Vector3 newStartPos = StageManager::GetInstance()->GetStageStartPos();
                pPlayer_->SetPlayerPos(newStartPos);

                if (pFade_->IsFadeInEnd())
                {
                    const StageID stage = StageManager::GetInstance()->GetCurrentStageID();
                    const GameSoundList bgm = StageToBgm(stage);

                    app::core::SoundManager::GetInstance()->PlayBGM(bgm);
                    m_hasAppliedStageBgm_ = true;
                    nextStageID_ = StageID::sInvalid;
                    state_ = SceneTransitionState::None;
                }

                state_ = SceneTransitionState::Load_WaitFinish;
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
                pPlayer_->SetPaused(false);
                pFade_->StartFadeIn();
                pNumberUI_->ResetTimer();
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

        void Game::UICreateInstance()
        {
            TimerCreateInstance();
            NumberCreateInstance();
            ScoreCreateInstance();
            HPbarCreateInstance();
        }

        void Game::PlayerCreateInstance()
        {
            pPlayer_ = NewGO<Player>(0, "player");
        }

        void Game::TimerCreateInstance()
        {
            pTimerUI_ = NewGO<TimerUI>(0, "timerui");
        }

        void Game::NumberCreateInstance()
        {
            pNumberUI_ = NewGO<NumberUI>(0, "numberui");
        }

        void Game::ScoreCreateInstance()
        {
            pScoreUI_ = NewGO<ScoreUI>(0, "scoreui");
        }

        void Game::HPbarCreateInstance()
        {
            pHpbarUI_ = NewGO<HPbarUI>(0, "hpbarui");
        }

    } // namespace core
} // namespace app
