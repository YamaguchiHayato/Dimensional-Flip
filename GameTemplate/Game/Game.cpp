#include "stdafx.h"
#include "ChaseEnemy.h"
#include "Door.h"	
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Stage1.h"

bool Game::Start()
{
    m_player = NewGO<Player>(0, "player");
	m_stage1 = NewGO<Stage1>(0, "stage1");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//EnemyNewGO(); // í«è]ìGÇÃê∂ê¨ÅB
	m_chaseEnemy = FindGO<ChaseEnemy>("chaseEnemy");
	
//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}


/// <summary>
///	í«è]ìGÇÃê∂ê¨ÅB
/// </summary>
void Game::EnemyNewGO()
{
	m_chaseEnemy = NewGO<ChaseEnemy>(0, "chaseEnemy");
	m_chaseEnemy->SetPosition(Vector3{ 200.0f, 0.0f, -0.0f });// í«è]ìGÇÃç¿ïWê›íËÅB
	m_chaseEnemy->m_EnemyPos = m_chaseEnemy->m_ChaseEnemyPos;
	m_modelRender.SetPosition(m_pos);

}

void Game::DoorNewGO()
{
	m_door = NewGO<Door>(0,"door");
// 	m_door-
}
