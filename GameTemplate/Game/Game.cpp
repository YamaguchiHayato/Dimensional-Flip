#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Stage1.h"
#include "TrackingEnemy.h"

namespace EnemyPosition
{
	const Vector3 pos(600.0f, 0.0f, 0.0f);
}

bool Game::Start()
{
    m_player = NewGO<Player>(0, "player");
	m_stage1 = NewGO<Stage1>(0, "stage1");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_gameCamera->Start();                 // © –Y‚ê‚¸ŒÄ‚Ô

//	EnemyNewGO_Tracking();
//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::EnemyNewGO_Tracking()
{
	m_trackingEnemy = NewGO<TrackingEnemy>(0, "TrackingEnemy");
	m_trackingEnemy->m_EnemyPosition = {EnemyPosition::pos};
	m_trackingEnemy->m_EnemyFP = m_trackingEnemy->m_EnemyPosition;
}
