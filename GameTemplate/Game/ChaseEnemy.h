//#pragma once
//
///// <summary>
//// Šî’êƒNƒ‰ƒX : IEnemy
///// </summary>
//class IEnemy
//{
//public:
//	IEnemy() {};
//	virtual ~IEnemy() {};
//	virtual bool Start() = 0;
//	virtual void Update() = 0;
//	virtual void Render(RenderContext & rc) = 0;
//
//private:
//	ChaseEnemy* m_chaseEnemy = nullptr; // ’Ç]“GB
//};
//
//class Enemy : public IGameObject
//{
//public:
//	Enemy() {};
//	~Enemy() {};
//	bool Start();
//	void Update();
//};
//
///// <summary>
///// ”h¶ƒNƒ‰ƒX : ChaseEnemy
///// </summary>
//class ChaseEnemy : public IEnemy
//{
//public:
//	ChaseEnemy() {};
//	~ChaseEnemy() {};
//
//	bool Start() override;
//	void Update() override;
//	void Render(RenderContext& rc) override;
//
//private:
//};
//
