#include "stdafx.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Scene/SceneManager.h"

SceneManager* SceneManager::pSceneManger_ = nullptr;



bool SceneManager::Start()
{
    // 最初のシーンを生成。
    pCurrentScene_ = CreateScene(SceneID::sTitle);
    if (pCurrentScene_)
    {
        pCurrentScene_->Start();
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
    switch (id)
    {
        // ケース: タイトルシーン。
        case SceneID::sTitle:
             pCurrentScene_= new TitleScene();
             currentID_ = SceneID::sInvalid;
             break;

         // ケース: インゲームシーン。
         case SceneID::sInGame:
              pCurrentScene_ = new InGameScene();
              currentID_ = SceneID::sInvalid;
              break;

          default:
              break;
    }

    return pCurrentScene_;
 }



