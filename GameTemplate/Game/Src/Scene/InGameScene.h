#pragma once
#include "Src/Scene/Scene.h"
namespace app
{
    namespace core
    {
        class Game;
    }
} 

class InGameScene : public IScene
{
public:
    InGameScene() {};
    virtual ~InGameScene();


public:
    bool Start() override;
    void Update() override;



private:
    app::core::Game* pGame_ = nullptr;
};
