#pragma once

/// <summary>
/// Enemyの派生クラスをnewするクラス。
/// </summary>
class Player;
class TrackingEnemy;
class EnemyFactory : public IGameObject
{
public:
	TrackingEnemy* m_TrackingEnemy = nullptr;
};

