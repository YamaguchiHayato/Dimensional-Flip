#pragma once
#include "Src/Scene/Scene.h"
class Stage2Scene : public SceneBase
{
public:
	Stage2Scene() {};
	virtual ~Stage2Scene();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

};

