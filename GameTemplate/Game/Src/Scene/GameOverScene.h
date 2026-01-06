#pragma once
#include "Src/Scene/Scene.h"


class GameOver;


namespace app
{
    namespace scene
    {
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
