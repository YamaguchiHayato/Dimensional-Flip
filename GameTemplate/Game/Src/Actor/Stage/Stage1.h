#pragma once
#include "Src/Actor/Stage/Stage.h"



class Stage1 : public IStage
{
public:
	Stage1() {};
	~Stage1();
	bool Start()override;
	void Update()override;
	void Render(RenderContext & rc)override;
	const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};

};

