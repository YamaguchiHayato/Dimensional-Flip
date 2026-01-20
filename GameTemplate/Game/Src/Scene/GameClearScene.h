#pragma once
#include "Src/Production/GameClear.h"
#include "Src/Scene/Scene.h"

class GameClearScene : public IScene
{
public:
    GameClearScene() = default;
    virtual ~GameClearScene();


public:
    bool Start()override;
    void Update()override;


private:
    GameClear* pGameClear_ = nullptr;
};
