#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"
#include "Src/Production/GameClear.h"
#include "Src/Scene/EndRollScene.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Scene/GameOverScene.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Scene/LoadingScene.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/WorldSelectScene.h"
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
    pFade_ = NewGO<Fade>(2, "fade");
    if (pFade_ == nullptr)
        return false;

    pCurrentScene_ = CreateScene(SceneID::sTitle);
    if (pCurrentScene_)
    {
        pCurrentScene_->Start();
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
            ShowLoading();
    }

    if (requestID_ != SceneID::sInvalid && !isLoadingSceneActive_)
    {
        ShowLoading();
        isLoadingSceneActive_ = true;
        isSceneControler_ = false;
        minLoadingTime_ = 0.0f;
        return;
    }

    if (isLoadingSceneActive_)
    {
        if (!isSceneControler_)
        {
            IScene* nextScene = CreateScene(requestID_);

            if (nextScene != nullptr)
            {
                // インゲーム描画状態を解除
                if (g_renderingEngine)
                {
                    g_renderingEngine->EnableCompositeBackground(false);
                    g_renderingEngine->SetStageBackGroundRenderer(nullptr);
                }

                // 旧シーン破棄 → GO 完全削除 → 新シーン開始（全シーン共通）
                if (pCurrentScene_)
                {
                    delete pCurrentScene_;
                    pCurrentScene_ = nullptr;
                }
                nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects();
                nextScene->Start();

                pCurrentScene_ = nextScene;
                currentID_ = requestID_;
            }

            isSceneControler_ = true;
        }

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

    if (pCurrentScene_ != nullptr)
        pCurrentScene_->Update();
}

void SceneManager::ShowLoading()
{
    if (pLoadingScene_ == nullptr)
        pLoadingScene_ = NewGO<LoadingScene>(3, "loading");
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
    case SceneID::sTitle:
        newScene = new TitleScene();
        break;

    case SceneID::sWorldSelect:
        newScene = new WorldSelectScene();
        break;

    case SceneID::sInGame:
        newScene = new InGameScene();
        break;

    case SceneID::sResult:
        newScene = new GameClearScene();
        break;

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
