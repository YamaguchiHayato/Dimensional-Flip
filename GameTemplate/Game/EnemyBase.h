#pragma once
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
	TrackingEnemy* m_TrackingEnemy = nullptr;// í«è]ìGÅB


};

class EnemyBase : public IGameObject ,public IEnemy
{
public:
	EnemyBase() {};
	~EnemyBase() {};

	bool Start()override { return true; };
	void Update() override {};
	void Render(RenderContext& rc)override {};

public:
	CharacterController m_EnemyCC;
	Vector3 m_EnemyPosition;
	ModelRender m_EnemyRender;

};

