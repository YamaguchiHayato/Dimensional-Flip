#pragma once
#include "Src/Scene/Scene.h"
class Fade;
class FadeScene : public IScene
{
public:
    FadeScene() {};
    virtual ~FadeScene() {};

    bool Start() override;
    void Update() override;

private:
    Fade* pFade_ = nullptr;
};
