#pragma once
#include "Src/Actor/Actor.h"

/// <summary>
/// Enemyの派生クラスをnewするクラス。
/// </summary>
class Player;
class TrackingEnemy;
class EnemyFactory : public Actor
{
public:
	TrackingEnemy* m_TrackingEnemy = nullptr;
};

