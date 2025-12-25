#include "stdafx.h"
#include "Game.h"
#include "Src/Direction/Fade.h"

#include "GameSoundEngine.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Scene/LoadingScene.h"
// UI。
#include "Src/UI/UIBase.h"
#include "Src/UI/TimerUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/HPbarUI.h"

// マネージャー。
#include "Src/Core/CameraManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Core/SoundManager.h"

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
            ///////////////////////////////////////////
            // StageManagerはシングルトンのため未破棄。
            ///////////////////////////////////////////

            DeleteGO(pSkyCube_);

            if (pLoadingScene_)
            {
                DeleteGO(pLoadingScene_);
                pLoadingScene_ = nullptr;
            }

            StageManager::DeleteInstance();
            SoundManager::DeleteInstence();
        }


        void Game::InitSkyCube()
        {
            DeleteGO(pSkyCube_);
            SkyCube* m_SkyCube = NewGO<SkyCube>(0, "skycube");

            // 環境光の計算のためのIBLテクスチャをセットする。
            g_renderingEngine->SetAmbientByIBLTexture(m_SkyCube->GetTextureFilePath(), 1.0f);
            // 環境日光の影響が分かりやすいように、ディレクションライトはオフに。
            g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);
        }


        bool Game::Start()
        {
            // StageManagerの生成。
            StageManager::CreateInstance();
            StageManager::GetInstance()->Start();

            app::core::SoundManager::CreateInstence();
            app::core::SoundManager::GetInstance()->Init();


            // UIの生成。
            UICreateInstance();

            // Playerの生成。
            PlayerCreateInstance();

            // CameraManagerの生成。
            pCameraManager_ = std::unique_ptr<CameraManager>(NewGO<CameraManager>(0, "cameramanager"));
            pPlayer_->InitCameraManager(pCameraManager_.get());


            // SceneManagerから Fade を取得。
            pFade_ = SceneManager::GetInstance()->GetFade();
            if (pFade_ == nullptr)
                return false;

            // 遷移ステートを初期化。
            state_ = SceneTransitionState::None;
            nextStageID_ = StageID::sInvalid;
            pLoadingScene_ = nullptr;


            // Playerの初期位置設定。
//            pPlayer_->SetPlayerPos(Vector3::Zero);
            pPlayer_->SetPlayerPos(Vector3(800.0f, 0.0f, 0.0f));
           
            InitSkyCube();

            // 物理デバッグワイヤーフレーム表示有効化。
            //  PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
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

            if (!pFade_->IsFadeInEnd())
            {
                return;
            }

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
        }


        void Game::RequestStageTransition(StageID nextStageID)
        {
            // 遷移中でければばリクエストを受け付ける。
            if (state_ == SceneTransitionState::None)
            {
                nextStageID_ = nextStageID;
                m_hasAppliedStageBgm_ = false;
                // フェードアウト開始。
                state_ = SceneTransitionState::FadeOut;
            }
        }


        void Game::UpdateTransition()
        {
            switch (state_)
            {
            /////////////////////////////////////////
            // A. 通常状態。
            /////////////////////////////////////////
            case SceneTransitionState::None:
                // 何もしない。
                break;

            /////////////////////////////////////////
            // B. フェードアウト状態。
            /////////////////////////////////////////
            case SceneTransitionState::FadeOut:
            {
                // プレイヤーを一時停止状態にする。
                pPlayer_->SetPaused(true);

                // フェードアウト開始。
                pFade_->StartFadeOut();

                // ストップウォッチタイマー開始。
                stageClearTimer_.Start();

                // 次の状態へ。
                state_ = SceneTransitionState::Load; // Cへ遷移。
                break;
            }

            /////////////////////////////////////////
            // C. ローディング状態。
            /////////////////////////////////////////
            case SceneTransitionState::Load:
            {
                // ストップウォッチを停止して、現在までの経過時間を計算。
                stageClearTimer_.Stop();

                // 1.FadeOutの完了を待つ。
                //        if (pFade_->IsFadeOutEnd() && stageClearTimer_.GetElapsed() >= 3.0f)
                if (pFade_->IsFadeOutEnd() && stageClearTimer_.GetElapsed() >= 3.0f)
                {
                    // 2. Loading画面を表示する。
                    pLoadingScene_ = NewGO<LoadingScene>(0, "LoadingScene");

                    // 3. ロードが終わったらFadeInへ
                    state_ = SceneTransitionState::Load_Render; // Dへ遷移。
                }
                break;
            }

            /////////////////////////////////////////
            // D. ローディング描画。
            /////////////////////////////////////////
            case SceneTransitionState::Load_Render:
            {
                state_ = SceneTransitionState::Load_Wait; // Eへ遷移。
                break;
            }

            /////////////////////////////////////////
            // E. 同期ロード状態。
            /////////////////////////////////////////
            case SceneTransitionState::Load_Wait:
            {
                // このフレームでGame->Renderが呼ばれ、ローディング画面を描画。
                // 次フレームで同期ロードを開始する。。

                // 3. 同期ロードを実行。
                StageManager::GetInstance()->ChangeStageSync(nextStageID_);


                // 4. ロード完了後、PlayerとCameraをリセット。
                Vector3 newStartPos = StageManager::GetInstance()->GetStageStartPos();
                pPlayer_->SetPlayerPos(newStartPos);

                if (pFade_->IsFadeInEnd())
                {
                    const StageID stage = StageManager::GetInstance()->GetCurrentStageID();
                    const GameSoundList bgm = StageToBgm(stage);

                    app::core::SoundManager::GetInstance()->PlayBGM(bgm); // ←ここで切替
                    // PlayBGMは前のBGMを止める実装になってるので基本これでOK :contentReference[oaicite:5]{index=5}
                    m_hasAppliedStageBgm_ = true;
                    nextStageID_ = StageID::sInvalid;
                    state_ = SceneTransitionState::None;
                }

                // 5. 物理エンジンを1フレーム更新するためFadeInステートへ。
                state_ = SceneTransitionState::Load_WaitFinish;
                break;
            }

            /////////////////////////////////////////
            // F.ロード完了待ち状態。
            /////////////////////////////////////////
            case SceneTransitionState::Load_WaitFinish:
            {
                // なにも処理を行わずに1フレーム待つ。
                state_ = SceneTransitionState::FadeIn;
                break;
            }

            /////////////////////////////////////////
            // G. フェードイン状態。
            /////////////////////////////////////////
            case SceneTransitionState::FadeIn:
            {
                // ローディング画面の削除。
                if (pLoadingScene_ != nullptr)
                {
                    DeleteGO(pLoadingScene_);
                    pLoadingScene_ = nullptr;
                }
                // プレイヤーの一時停止解除。
                pPlayer_->SetPaused(false);

                // フェードイン開始。
                pFade_->StartFadeIn();

                // TimerUIをリセットする。
                pNumberUI_->ResetTimer();

                // フェード完了待ちステートへ移行。
                state_ = SceneTransitionState::FadeIn_Wait;
                break;
            }

            /////////////////////////////////////////
            // H. フェードイン状態。
            /////////////////////////////////////////
            case SceneTransitionState::FadeIn_Wait:
            {
                if (pFade_->IsFadeInEnd())
                {
                    nextStageID_ = StageID::sInvalid;
                    state_ = SceneTransitionState::None; // ここで初めてゲーム開始！
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
    }
}
