#pragma once
#include "Src/Title.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Stage/Stage2.h"
class Title;
class Player;
class Stage1;
class Stage2;
class SceneBase :public IGameObject
{
public:
	SceneBase() {};
	virtual ~SceneBase() {};

	virtual bool Start() { return true; };
	virtual void Update();
	virtual void Render(RenderContext& rc) {};

	Title* title_ = nullptr;
	Stage1* stage1_ = nullptr;
	Stage2* stage2_ = nullptr;
	Player* player_ = nullptr;
};

