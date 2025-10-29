#pragma once
#include "Src/Scene/Scene.h"
// Characterクラス。
class Player;
// Gimmickクラス。
class WallActor;
class Star;
// Stageクラス。
class Stage1;
class StageClear;
class Stage1Scene : public IScene
{
public:
	Stage1Scene() {};
	virtual ~Stage1Scene();
	bool Start() override;
	void Update() override;


private:
    Player* pPlayer_ = nullptr;
    Star* pStar_ = nullptr;
    Stage1* pStage1_ = nullptr;
    StageClear* pStageClear_ = nullptr;

};

