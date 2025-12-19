#pragma once
#include "Src/Direction/GameClear.h"
#include "Src/Scene/Scene.h"

class GameClearScene : public IScene
{
public:
    GameClearScene() = default;
    virtual ~GameClearScene() = default;

    bool Start()override;
    void Update()override;
};
