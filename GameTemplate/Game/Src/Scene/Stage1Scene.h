#pragma once
#include "Scene.h"
class StageClear;
class Stage1Scene : public SceneBase
{
public:
	Stage1Scene() {};
	virtual ~Stage1Scene() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override {};

private:
    StageClear* stageClear_ = nullptr;
};

