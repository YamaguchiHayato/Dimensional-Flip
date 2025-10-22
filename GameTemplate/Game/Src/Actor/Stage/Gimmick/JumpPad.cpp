#include "stdafx.h"
#include "JumpPad.h"
#include "Src/Actor/Character/Player.h"

namespace
{
	const float JUMPPAD_JUMP_POWER = 50.0f;
	const Vector3 JUMPPAD_SCALE(0.2f, 0.2f, 0.2f);
	const Vector3 JUMPPAD_POSITION(500.0f, -20.0f, 0.0f);

	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(365.0f, 5.0f, 225.0f);//コリジョンの大きさ
}

JumpPad::~JumpPad()
{
	DeleteGO(m_jumpPadCollision);
}

bool JumpPad::Start()
{
	m_jumpPadModelRender.Init("Assets/stage/Stage1/JumpPad.tkm");
	m_jumpPadModelRender.SetScale(JUMPPAD_SCALE);

	m_jumpPadModelRender.Update();

	m_player = FindGO<Player>("player");

	m_jumpPadPhysics.CreateFromModel
	(
		m_jumpPadModelRender.GetModel(),
		m_jumpPadModelRender.GetModel().GetWorldMatrix()
	);

	m_jumpPadCollision = NewGO<CollisionObject>(0, "collisionobject");


	m_jumpPadCollision->CreateBox
	(
		m_jumpPadPosition + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE
	);

	m_jumpPadModelRender.SetPosition(m_jumpPadPosition);

	m_jumpPadCollision->SetIsEnableAutoDelete(false);
	m_padFirstPosition = m_jumpPadPosition;
	return true;
}

void JumpPad::Update()
{
	if (m_player == nullptr) return;

	LaunchPlayer();
	m_jumpPadPhysics.SetPosition(m_jumpPadPosition);

	m_jumpPadCollision->SetPosition(m_jumpPadPosition + COLLISION_HEIGHT);

	m_jumpPadModelRender.Update();

}

void JumpPad::LaunchPlayer()
{
	if (m_jumpPadCollision->IsHit(m_player->GetCharacterController()))
	{
		m_player->moveSpeed_.y = 400.0f;
	}

	else { return; }
}

void JumpPad::Render(RenderContext& rc)
{
	m_jumpPadModelRender.Draw(rc);
}
