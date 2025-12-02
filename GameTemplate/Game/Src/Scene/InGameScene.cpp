#include "stdafx.h"
#include "Src/Core/Game.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"

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

