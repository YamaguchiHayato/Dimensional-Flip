#include "stdafx.h"
#include "Src/Core/Game.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/production/Fade.h"


InGameScene::~InGameScene()
{
    // Gameオブジェクトの削除。
    DeleteGO(pGame_);
}

bool InGameScene::Start()
{
    pGame_ = NewGO<app::core::Game>(0, "game");

    SceneManager::GetInstance()->GetFade()->StartFadeIn();
    return true;
}


// Src/Scene/InGameScene.cpp

void InGameScene::Update()
{
    // フェードアウトが完了したらシーン遷移
    Fade* fade = SceneManager::GetInstance()->GetFade();
    if (fade && fade->IsFadeOutEnd())
    {
        if (pGame_->GetNextStageID() == StageID::sInvalid)
        {
            SceneManager::GetInstance()->ChangeScene(SceneID::sResult);
        }

        else
        {
            // 念のため、その他のケース（通常はここに来ないはず）
            SceneManager::GetInstance()->ChangeScene(SceneID::sResult);
        }
    }
}
