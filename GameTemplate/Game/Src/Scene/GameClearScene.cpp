#include "stdafx.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"

#include "Src/Core/StageManager.h"

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

    const auto& data = app::core::StageManager::GetStageResultData();
    pGameClear_->SetUpResultData(data);

    SceneManager::GetInstance()->GetFade()->StartFadeIn();
    return true;
}


void GameClearScene::Update()
{
    if (pGameClear_ && pGameClear_->IsFinished())
    {
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            // 演出が終わっていて、かつボタンが押されていたら。
            SceneManager::GetInstance()->ChangeScene(SceneID::sEndRoll);
        }
    }
}
