#pragma once
#include "Src/Actor/Actor.h"

class Player;
class JumpPad : public Actor
{
public:
	JumpPad() {};
	virtual ~JumpPad();

	/* 初期化処理。*/ 
	bool Start()override;
	/* 更新処理。*/
	void Update()override;
	/* 描画処理。*/
	void Render(RenderContext& rc)override;

	/* 
	 * @brief JumpPadとしてのコア処理。
	 */
	void LaunchPlayer();
	/* 
	 * @brief ジャンプパッドの座標設定。
	 */
	const void SetJumpPadPosition(const Vector3& jumpPadPosition)
	{
		m_jumpPadPosition = jumpPadPosition;
	}

private:
	Player* m_player = nullptr;
	CollisionObject* m_jumpPadCollision = nullptr;

private:
	ModelRender m_jumpPadModelRender;
	Vector3 m_padFirstPosition;
	Vector3 m_jumpPadPosition;
	PhysicsStaticObject m_jumpPadPhysics;
private:
	float m_jumpPower = 50.0f;

};

