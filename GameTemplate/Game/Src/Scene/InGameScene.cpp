#include "stdafx.h"
#include "Game.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Scene/SceneManager.h"
#include "Src/Actor/Stage/StageManager.h"

InGameScene::~InGameScene()
{
    // Gameオブジェクトの削除。
    DeleteGO(pGame_);
}

bool InGameScene::Start()
{
    pGame_ = NewGO<Game>(0, "game");
    return true;
}

