#include "stdafx.h"

#include "Game.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/production/Fade.h"
#include "Src/Scene/LoadingScene.h"
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
            // SkyCubeを削除。
            if (pSkyCube_)
            {
                DeleteGO(pSkyCube_);
                pSkyCube_ = nullptr;
            }

            // ステージ背景を削除。
            if (pBackGround_)
            {
                DeleteGO(pBackGround_);
                pBackGround_ = nullptr;
            }

            // ローディング画面を削除。
            if (pLoadingScene_)
            {
                DeleteGO(pLoadingScene_);
                pLoadingScene_ = nullptr;
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


        void Game::InitSkyCube()
        {
            pSkyCube_ = NewGO<SkyCube>(0, "skycube");

            pSkyCube_->SetScale(Vector3::One * 100.0f);

            // IBLテクスチャを設定。
            g_renderingEngine->SetAmbientByIBLTexture(pSkyCube_->GetTextureFilePath(), 1.0f);

            // SkyCubeのタイプを設定。
            pSkyCube_->SetType(EnSkyCubeType::enSkyCubeType_Day);
            g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);
        }


        bool Game::Start()
        {
            srand(static_cast<unsigned int>(time(nullptr)));

            // ステージの生成。
            StageManager::CreateInstance();

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
            nextStageID_ = StageID::sInvalid;
            pLoadingScene_ = nullptr;

            // Playerの初期位置設定。
            // 初期地点
           pPlayer_->SetPlayerPos(Vector3(0.0f, 20.0f, 0.0f));

            // ステージ背景の生成。
            CreateBackGround();

            // SkyCubeの初期化。
            InitSkyCube();

            // 物理デバッグワイヤーフレーム表示有効化。
            PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
            pFade_ = SceneManager::GetInstance()->GetFade();


            return true;
        }


        void Game::Update()
        {
            // フェードイン開始。
            if (!m_isFadeInEnd)
            {
                pFade_->StartFadeIn();

                m_isFadeInEnd = true;
            }


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


                // 背景の切り替え更新。
                if (pCameraManager_ && pSkyCube_)
                {
                    if (pCameraManager_->GetCurrentCameraMode() == CameraMode::mode2D)
                        pSkyCube_->SetScale(0.0f);

                    else
                        pSkyCube_->SetScale(2000.0f);
                }
            }


            if (pPlayer_ && pPlayer_->GetHP() <= 0)
            {
                SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
                return;
            }
        }


        void Game::RequestStageTransition(StageID nextStageID)
        {
            // 遷移中でければばリクエストを受け付ける。
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
                // 2Dへ。
                collision.SetDimension(collision::DimensionMode::dim2D);

            else
                // 3Dへ。
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
                if (pFade_->IsFadeOutEnd() && stageClearTimer_.GetElapsed() >= 3.0f)
                {
                    pLoadingScene_ = NewGO<LoadingScene>(0, "LoadingScene");
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
                if (pLoadingScene_ != nullptr)
                {
                    DeleteGO(pLoadingScene_);
                    pLoadingScene_ = nullptr;
                }
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


        void Game::CreateBackGround()
        {
            // 背景画像の生成。
            pBackGround_ = NewGO<app::stage::BackGround>(0, "background");
        }
    } // namespace core
} // namespace app
