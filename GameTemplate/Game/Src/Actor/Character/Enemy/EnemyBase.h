#pragma once
#include "Src/Actor/Actor.h"
class TrackingEnemy;
class IEnemy
{
public:
	IEnemy() {};
	virtual ~IEnemy() {};
	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

public:
	TrackingEnemy* pTrackingEnemy_ = nullptr;// 追従敵。


};

class EnemyBase : public Actor
{
public:
	EnemyBase() {};
	~EnemyBase() {};

	bool Start()override { return true; };
	void Update() override {};
	void Render(RenderContext& rc)override {};

public:
	CharacterController enemyCC_;
	Vector3 enemyPosition_;
	ModelRender enemyRender_;

};

