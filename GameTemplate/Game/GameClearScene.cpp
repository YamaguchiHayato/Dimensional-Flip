#include "stdafx.h"
#include "GameClearScene.h"

bool GameClearScene::Start()
{
    NewGO<GameClear>(0, "gameClear");
    return true;
}

void GameClearScene::Update()
{

}
