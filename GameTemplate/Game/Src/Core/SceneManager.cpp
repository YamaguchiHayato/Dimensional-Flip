#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/EndRollScene.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Scene/GameOverScene.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/WorldSelectScene.h"
#include "Src/Core/StageManager.h"

namespace
{
    void DestroyNamedGO(const char* name)
    {
        if (auto* go = FindGO<IGameObject>(name))
            DeleteGO(go);
    }

    // シーン delete + Flush の「後」だけ呼ぶ
    void CleanupOrphanGameObjects()
    {
        SceneManager::ResetRenderingStateForScene();

        DestroyNamedGO("skycube");
        DestroyNamedGO("SkyCube");
        DestroyNamedGO("WorldSelectUI");
        DestroyNamedGO("player");
        DestroyNamedGO("timerui");
        DestroyNamedGO("numberui");
        DestroyNamedGO("scoreui");
        DestroyNamedGO("hpbarui");
        DestroyNamedGO("cameramanager");
        DestroyNamedGO("BackGround");
        DestroyNamedGO("EndRollManager");
        DestroyNamedGO("boss");
        DestroyNamedGO("game");
        DestroyNamedGO("stagemanager");
        DestroyNamedGO("stage");
        DestroyNamedGO("dimensiontrigger");
        DestroyNamedGO("CutInView");

        QueryGOs<IGameObject>("StageIcon", [](IGameObject* go) { DeleteGO(go); return true;});

        // StageManager::DeleteInstance();  ← 削除

        nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects();
    }
} // namespace

namespace nsApp
{
    namespace nsCore
    {
        SceneManager* SceneManager::pSceneManger_ = nullptr;


        SceneManager::~SceneManager()
        {
            /**
             * @brief 現在シーンの C++ オブジェクトを破棄する
             */
            if (pCurrentScene_ != nullptr)
            {
                delete pCurrentScene_;
                pCurrentScene_ = nullptr;
            }

            /**
             * @brief フェード GO を削除予約する
             */
            if (pFade_ != nullptr)
            {
                DeleteGO(pFade_);
                pFade_ = nullptr;
            }
        }

        bool SceneManager::Start()
        {
            /**
             * @brief 全シーン共通のフェード GO を生成する
             */
            pFade_ = NewGO<nsProduction::Fade>(2, "fade");
            if (pFade_ == nullptr)
                return false;

            /**
             * @brief 起動シーンとしてタイトルを構築・開始する
             */
            pCurrentScene_ = CreateScene(nsScene::SceneID::sTitle);
            if (pCurrentScene_)
            {
                pCurrentScene_->Start();
                currentID_ = nsScene::SceneID::sTitle;
                return true;
            }
            return false;
        }

        void SceneManager::Update()
        {
            /**
             * @brief フェードアウト途中でロード画面を先出しする
             */
            if (isAutoLoadingEnabled_ && pFade_ && pFade_->GetFadeState() == FadeState::Fade_Out)
            {
                if (pFade_->GetFadeSprite().GetWipeSize() < 300.0f)
                    ShowLoading();
            }

            /**
             * @brief 遷移リクエストを受けたらロード状態へ入る
             */
            if (requestID_ != nsScene::SceneID::sInvalid && !isLoadingSceneActive_)
            {
                ShowLoading();
                isLoadingSceneActive_ = true;
                isSceneControler_ = false;
                minLoadingTime_ = 0.0f;
                return;
            }

            if (isLoadingSceneActive_)
            {
                /**
                 * @brief ロード中の初回のみシーン切替を実行する
                 */
                if (!isSceneControler_)
                {
                    nsScene::IScene* nextScene = CreateScene(requestID_); //! 遷移先シーンのインスタンス

                    if (nextScene != nullptr)
                    {
                        /**
                         * @brief インゲーム専用の描画状態を解除する
                         */
                        if (g_renderingEngine)
                        {
                            g_renderingEngine->EnableCompositeBackground(false);
                            g_renderingEngine->SetStageBackGroundRenderer(nullptr);
                        }

                        /**
                         * @brief 旧シーン破棄 → GO 完全削除 → 新シーン開始
                         */
                        if (pCurrentScene_)
                        {
                            delete pCurrentScene_; 
                            pCurrentScene_ = nullptr;
                        }
                        nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects(); 

                        CleanupOrphanGameObjects(); 

                        nextScene->Start();
                        nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects();


                        pCurrentScene_ = nextScene;
                        currentID_ = requestID_;
                    }

                    isSceneControler_ = true;
                }

                float dt = g_gameTime->GetFrameDeltaTime(); //! フレーム経過時間
                if (dt > 0.1f)
                    dt = 0.1f;

                minLoadingTime_ += dt;

                /**
                 * @brief 最低表示時間を満たしたらロード UI を閉じる
                 */
                if (minLoadingTime_ >= 0.5f)
                {
                    requestID_ = nsScene::SceneID::sInvalid;
                    isLoadingSceneActive_ = false;
                    HideLoading();
                }
            }

            /**
             * @brief 現シーンの更新処理を呼ぶ
             */
            if (pCurrentScene_ != nullptr)
                pCurrentScene_->Update();
        }


        void SceneManager::ShowLoading()
        {
            /**
             * @brief 未生成ならロード画面 GO を作る
             */
            if (pLoadingScene_ == nullptr)
                pLoadingScene_ = NewGO<nsScene::LoadingScene>(3, "loading");
        }


        void SceneManager::HideLoading()
        {
            /**
             * @brief ロード画面 GO を削除する
             */
            if (pLoadingScene_ != nullptr)
            {
                DeleteGO(pLoadingScene_);
                pLoadingScene_ = nullptr;
            }
        }


        nsScene::IScene* SceneManager::CreateScene(nsScene::SceneID id)
        {
            nsScene::IScene* newScene = nullptr; //! 生成したシーンの返却用

            switch (id)
            {
            case nsScene::SceneID::sTitle:
                newScene = new nsScene::TitleScene();
                break;

            case nsScene::SceneID::sWorldSelect:
                newScene = new nsScene::WorldSelectScene();
                break;

            case nsScene::SceneID::sInGame:
                newScene = new nsScene::InGameScene();
                break;

            case nsScene::SceneID::sResult:
                newScene = new nsScene::GameClearScene();
                break;

            case nsScene::SceneID::sGameOver:
                newScene = new nsScene::GameOverScene();
                break;

           case nsScene::SceneID::sEndRoll:
                newScene = new nsScene::EndRollScene();
                break;
            default:
                break;
            }
            return newScene;
        }
    } // namespace nsCore
} // namespace nsApp
