#pragma once
#include "Src/Scene/Scene.h"

class Fade;
class Title;
class TitleScene : public IScene
{
public:
	TitleScene() {};
	virtual ~TitleScene();
	bool Start() override;
	void Update() override;

private:
    Fade* pFade_ = nullptr;
	Title* pTitle_ = nullptr;

};

