#include "stdafx.h"
#include "GameClearScene.h"


namespace app
{
    namespace scene
    {
        bool GameClearScene::Start()
        {
            NewGO<GameClear>(0, "gameClear");
            return true;
        }


        void GameClearScene::Update()
        {

        }
    }
}
