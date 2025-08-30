#pragma once
#include "Level3DRender/LevelRender.h"

class Player;
class Stage1;
class GameCamera;
class Game : public IGameObject
{
public:
	Game() {};
	~Game() {};
	bool Start();

private:
	Player* m_player = nullptr;
	Stage1* m_stage1 = nullptr;
	GameCamera* m_gameCamera = nullptr;
private:
	ModelRender m_modelRender;
	Vector3 m_pos;
};

