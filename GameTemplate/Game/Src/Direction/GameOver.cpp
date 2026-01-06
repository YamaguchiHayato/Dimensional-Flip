#include "stdafx.h"
#include "GameOver.h"


namespace app
{
    namespace direction
    {
        bool GameOver::Start()
        {
            // 適切なGameOver画像パスを指定してください
            sprite_.Init("Assets/sprite/GameOver.DDS", 1920.0f, 1080.0f);
            return true;
        }


        void GameOver::Update()
        {
            sprite_.Update();
        }


        void GameOver::Render(RenderContext& rc)
        {
            sprite_.Draw(rc);
        }
    }
}
