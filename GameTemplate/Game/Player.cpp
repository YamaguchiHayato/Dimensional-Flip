#include "stdafx.h"
#include "Player.h"

namespace
{
	// プレイヤーモデルの大きさ。
	const Vector3 SCALE(0.5f, 0.5f, 0.5f);
	// モデルにかかる重力。
	const float GLAVITY = 15.0f;
	// 移動速度。
}

bool Player::Start()
{
	animationClip[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	animationClip[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	animationClip[enAnimationClip_Walk].SetLoopFlag(true);
	animationClip[enAnimationClip_Run].Load("Assets/animData/run.tka");
	animationClip[enAnimationClip_Run].SetLoopFlag(true);
	animationClip[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	animationClip[enAnimationClip_Jump].SetLoopFlag(false);
	//モデルを初期化する。
	m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClip, enAnimationClip_Num, enModelUpAxisY);
	//キャラクターコントローラーの初期化
	m_Characon.Init(25.0f, 75.0f, m_position);

	m_position = Vector3(0, 0, 0);
	return true;
}

void Player::Update()
{
	Move();
	Rotation();
	PlayAnimation();
	m_modelRender.SetScale(SCALE);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void Player::Move()
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	right *= stickL.x * 480.0f;
	forward *= stickL.y * 0.0f;

	m_moveSpeed += right + forward;

	if(m_Characon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
		if(g_pad[0]->IsTrigger(enButtonA))
		{
			m_moveSpeed.y = 300.0f;
		}
	} 
	else
	{
		// 重力の発生(ステージ下への貫通を防ぐ)。
		m_moveSpeed.y -= GLAVITY;
	}
	m_position = m_Characon.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_position.z = 0.0f;


	m_Characon.SetPosition(m_position);

	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		rot.SetRotationYFromDirectionXZ(m_moveSpeed);
		m_modelRender.SetRotation(rot);
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
	case enPlayer_idle:
		 m_modelRender.PlayAnimation(enAnimationClip_Idle);
		 break;
	case enPlayer_walk:
		 m_modelRender.PlayAnimation(enAnimationClip_Walk);
		 break;
	case enPlayer_run:
		 m_modelRender.PlayAnimation(enAnimationClip_Run);
		 break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}