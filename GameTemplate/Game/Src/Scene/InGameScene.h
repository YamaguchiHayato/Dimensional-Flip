#pragma once
#include "Src/Scene/Scene.h"
class Game;
class InGameScene : public IScene
{
public:
    InGameScene() {};
    virtual ~InGameScene();

    bool Start() override;
    void Update() override;

private:
    Game* pGame_ = nullptr;
};
