#pragma once
#include "Src/Actor/Stage/Stage.h"
class Stage2 : public IStage
{
public:
	Stage2() {};
	virtual ~Stage2();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
};

