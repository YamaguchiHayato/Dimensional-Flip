#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/PlayerForward.h"

class JumpPad : public Actor
{
public:
	JumpPad() {};
	virtual ~JumpPad();

	/* �����������B*/ 
	bool Start()override;
	/* �X�V�����B*/
	void Update()override;
	/* �`�揈���B*/
	void Render(RenderContext& rc)override;

	/* 
	 * @brief JumpPad�Ƃ��ẴR�A�����B
	 */
	void LaunchPlayer();
	/* 
	 * @brief �W�����v�p�b�h�̍��W�ݒ�B
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

