#include "stdafx.h"
#include "Src/Direction/Fade.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Core/SceneManager.h"

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
            
        //// ゲーム開始時にFadeEndを開始する。s
        //pFade_->FadeTransition(FadeState::FadeEnd);
        return true;
    }
    return false;
} 

void SceneManager::Update()
{
    if (requestID_ != SceneID::sInvalid)
    {
        IScene* nextScene = CreateScene(requestID_);

        if (nextScene != nullptr)
        {
            nextScene->Start();
            // 前のシーンを消す。
            delete pCurrentScene_;

            // 次のシーンに切り替える。
            pCurrentScene_ = nextScene;
            // シーンIDを更新する。
            currentID_  = requestID_;

        }
        // シーンリクエストをクリア。
        requestID_ = SceneID::sInvalid;
    }


    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Update();
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

        // ケース: インゲームシーン。
        case SceneID::sInGame:
             newScene = new InGameScene();
             break;

        default:
             break;
    }

    return newScene;
 }



