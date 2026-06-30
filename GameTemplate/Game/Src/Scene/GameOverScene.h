#pragma once
#include "Src/Scene/Scene.h"

namespace nsApp
{
    namespace nsScene
    {
        /**
         * @class GameOverScene
         * @brief  ゲームオーバー画面シーン。
         */
        class GameOverScene : public IScene
        {
        public:
            GameOverScene() = default;
            virtual ~GameOverScene() = default;

            bool Start() override;
            void Update() override;
        };
    }
}

using GameOverScene = nsApp::nsScene::GameOverScene;
