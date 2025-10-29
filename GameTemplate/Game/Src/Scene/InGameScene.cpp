#include "stdafx.h"
#include "InGameScene.h"
#include "Game.h"

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

void InGameScene::Update()
{

}
