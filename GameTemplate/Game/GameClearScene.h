#pragma once
#include "Src/Direction/GameClear.h"
#include "Src/Scene/Scene.h"

namespace app
{
    namespace scene
    {
        class GameClearScene : public IScene
        {
        public:
            GameClearScene() = default;
            virtual ~GameClearScene() = default;

            bool Start() override;
            void Update() override;
        };

    }
}

