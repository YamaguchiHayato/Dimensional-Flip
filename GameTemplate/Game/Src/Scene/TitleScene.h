#pragma once
#include "Scene.h"
class Title;
class TitleScene : public SceneBase
{
public:
	TitleScene() {};
	~TitleScene() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override {};
private:
	Title* title_ = nullptr;
};

