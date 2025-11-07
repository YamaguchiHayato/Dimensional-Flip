#include "stdafx.h"
#include "InGameScene.h"
#include "Game.h"
#include "Fade.h"
#include "Src/Scene/SceneManager.h"
InGameScene::~InGameScene()
{
    // Gameオブジェクトの削除。
    DeleteGO(pGame_);
}

bool InGameScene::Start()
{
    pGame_ = NewGO<Game>(0, "game");

    pFade_ = SceneManager::GetInstance()->GetFade();

    if (pFade_ != nullptr)
    {
        pFade_->FadeTransition(FadeState::FadeStart);
    }
    return true;
}

