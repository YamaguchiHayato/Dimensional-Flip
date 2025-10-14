#pragma once
#include "SceneBase.h"
class Title;
class TitleScene : public SceneBase
{
public:
	TitleScene() {};
	~TitleScene() {};
	bool Start() override;
	void Update() override;

private:
	Title* title_ = nullptr;
};

