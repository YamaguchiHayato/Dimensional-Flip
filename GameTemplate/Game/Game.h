#pragma once
#include "Level3DRender/LevelRender.h"

class ChaseEnemy;
class Door;
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
	/// <summary>
	/// Enemyの生成。
	/// </summary>
	void EnemyNewGO();

	/// <summary>
	/// ドアの生成。
	/// </summary>
	void DoorNewGO();
private:
	// モデル。
	ChaseEnemy* m_chaseEnemy = nullptr;
	Player* m_player = nullptr;

	// ステージ。
	Stage1* m_stage1 = nullptr;
	Door* m_door = nullptr;

	// その他。
	GameCamera* m_gameCamera = nullptr;
	
private:
	ModelRender m_modelRender; // モデル。
	Vector3 m_pos;// 座標。
	Quaternion m_Rot;
};

