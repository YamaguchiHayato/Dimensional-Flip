#include "stdafx.h"
#include "Src/Production/Fade.h"

#include "Src/Scene/TitleScene.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/GameClear.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Scene/GameOverScene.h"
#include "Src/Scene/WorldSelectScene.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/Scene/EndRollScene.h"
#include "Src/UI/Select/WorldSelectUI.h"

SceneManager* SceneManager::pSceneManger_ = nullptr;


SceneManager::~SceneManager()
{
    if (pCurrentScene_ != nullptr)
    {
        delete pCurrentScene_;
        pCurrentScene_ = nullptr;
    }

    if (pFade_ != nullptr)
    {
        DeleteGO(pFade_);
        pFade_ = nullptr;
    }
}


bool SceneManager::Start()
{
    // フェードシーンの生成。
    pFade_ = NewGO<Fade>(2, "fade");
    if (pFade_ == nullptr)
    {
        return false;
    }
    // 最初のシーンを生成。
    pCurrentScene_ = CreateScene(SceneID::sTitle);
    if (pCurrentScene_)
    {
        pCurrentScene_->Start();
        // シーンの初期値を設定する。
        currentID_ = SceneID::sTitle;
        return true;
    }
    return false;
} 


void SceneManager::Update()
{
    if (isAutoLoadingEnabled_ && pFade_ && pFade_->GetFadeState() == FadeState::Fade_Out)
    {
        if (pFade_->GetFadeSprite().GetWipeSize() < 300.0f)
        {
            ShowLoading();
        }
    }

    // 2. シーン遷移リクエストがあった場合の初期化処理
    if (requestID_ != SceneID::sInvalid && !isLoadingSceneActive_)
    {
        // ロード画面を表示
        ShowLoading();

        // フラグとタイマーのリセット
        isLoadingSceneActive_ = true;
        isSceneControler_ = false;
        minLoadingTime_ = 0.0f;

        return; // ロード画面を描画するために一旦リターン
    }

    // 3. ロード中の処理
    if (isLoadingSceneActive_)
    {
        if (!isSceneControler_)
        {
            IScene* nextScene = CreateScene(requestID_);

            if (nextScene != nullptr)
            {
                if (g_renderingEngine)
                {
                    g_renderingEngine->EnableCompositeBackground(false);
                    g_renderingEngine->SetStageBackGroundRenderer(nullptr);
                }

                if (requestID_ == SceneID::sResult)
                {
                    nextScene->Start();

                    if (pCurrentScene_)
                    {
                        delete pCurrentScene_;
                        pCurrentScene_ = nullptr;
                    }
                }
                else
                {
                    if (pCurrentScene_)
                    {
                        delete pCurrentScene_;
                        pCurrentScene_ = nullptr;
                    }

                    nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects();
                    nextScene->Start();
                }

                pCurrentScene_ = nextScene;
                currentID_ = requestID_;
            }

            isSceneControler_ = true;
        }

        // B. 待機時間の更新
        float dt = g_gameTime->GetFrameDeltaTime();

        if (dt > 0.1f)
            dt = 0.1f;

        minLoadingTime_ += dt;

        if (minLoadingTime_ >= 0.5f)
        {
            requestID_ = SceneID::sInvalid;
            isLoadingSceneActive_ = false;
            HideLoading();
        }
    }

    // 4. 現在のシーンの更新
    if (pCurrentScene_ != nullptr)
        pCurrentScene_->Update();
}


void SceneManager::ShowLoading()
{
    if (pLoadingScene_ == nullptr)
    {
        // 優先度3（Fadeより手前）で生成
        pLoadingScene_ = NewGO<LoadingScene>(3, "loading");
    }
}


void SceneManager::HideLoading()
{
    if (pLoadingScene_ != nullptr)
    {
        DeleteGO(pLoadingScene_);
        pLoadingScene_ = nullptr;
    }
}



IScene* SceneManager::CreateScene(SceneID id)
{
    IScene* newScene = nullptr;
    switch (id)
    {
        // ケース: タイトルシーン。
        case SceneID::sTitle:
             newScene= new TitleScene();
             break;

        case SceneID::sWorldSelect:
             newScene = new WorldSelectScene();
             break;

        case SceneID::sInGame:
             newScene = new InGameScene();
             break;

        case SceneID::sResult:
             newScene = new GameClearScene();
             break
                 ;
        case SceneID::sGameOver:
             newScene = new app::scene::GameOverScene();
             break;

        case SceneID::sEndRoll:
             newScene = new app::production::EndRollScene();
             break;

        default:
             break;
    }

    return newScene;
 }
