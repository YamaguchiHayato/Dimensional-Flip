#include "stdafx.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Direction/Fade.h"


GameClearScene::~GameClearScene()
{
    if (pGameClear_)
    {
        DeleteGO(pGameClear_);
        pGameClear_ = nullptr;  
    }
}

bool GameClearScene::Start()
{
    pGameClear_= NewGO<GameClear>(0, "gameClear");

    SceneManager::GetInstance()->GetFade()->StartFadeIn();
    return true;
}

void GameClearScene::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
    }
}
