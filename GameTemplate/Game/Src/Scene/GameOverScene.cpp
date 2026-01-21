#include "stdafx.h"

#include "GameOverScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Production/GameOver.h"
#include "Src/Production/Fade.h"

namespace app
{
    namespace scene
    {
        bool GameOverScene::Start()
        {
            //　@TODO:  ゲームオーバー中のBGM停止。
            NewGO<app::direction::GameOver>(0, "gameOver");

            SceneManager::GetInstance()->GetFade()->StartFadeIn();
            return true;
        }


        void GameOverScene::Update()
        {
            // Aボタンでタイトルへ戻る
            if (g_pad[0]->IsTrigger(enButtonA))
            {
                // シーン遷移
                SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
            }
        }
    } 
} 
