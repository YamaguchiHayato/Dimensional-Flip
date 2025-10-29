#pragma once
#include "Src/Scene/Scene.h"
class Player;
class Stage2;
class Stage2Scene : public IScene
{
public:
	Stage2Scene() {};
	virtual ~Stage2Scene(){};

	bool Start() override{ return true; };
	void Update() override { };

private:
    Stage2* pStage2_ = nullptr;
    Player* pPlayer_ = nullptr;
};

