#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Stage1.h"

bool Game::Start()
{
    m_player = NewGO<Player>(0, "player");
	m_stage1 = NewGO<Stage1>(0, "stage1");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}
