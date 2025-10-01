#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/GameCamera.h"
namespace
{
	const Vector3 SCALE(0.5f, 0.5f, 0.5f);

	const float JUMP_INITIAL_SPEED = 2500.0f;
	const float GRAVITY = 20.0f;   
	const float JUMP_HOLD_GRAVITY = 7.0f;
	const float FALL_GRAVITY = 30.0f;  


	const char* PLAYER_MODEL = "Assets/modelData/";
	const char* PLAYER_ANIMATION = "Assets/animData/";// ファイルパス。
	const char* ANIMATION_FILE_EXTENSION = ".tka"; // 拡張子。
	const char* MODEL_FILE_EXTENSION = ".tkm"; // 拡張子。
}

const std::string Player::FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName, bool flag)
{
	std::string AnimationFilePath = PLAYER_ANIMATION + animationName + ANIMATION_FILE_EXTENSION;
    
	m_animationClip[enAnimationClip].Load(AnimationFilePath.c_str());
	m_animationClip[enAnimationClip].SetLoopFlag(flag);
	return AnimationFilePath;
}

const std::string Player::FetchPlayerModel(const std::string& modelName, AnimationClip animationClip, EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag)
{
	// モデルをロード(tkmファイル名を打ち込む)。
	std::string  Player = PLAYER_MODEL + modelName + MODEL_FILE_EXTENSION;

	m_animationClip[enAnimationClip].Load(Player.c_str());
	m_animationClip[enAnimationClip].SetLoopFlag(flag);
	return Player;

}

bool Player::Start()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClip[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClip[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClip[enAnimationClip_Jump].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClip, enAnimationClip_Num, enModelUpAxisY);

	m_Characon.Init(25.0f, 25.0f, m_position);

	m_position = Vector3(0, 0, 0);

	m_gameCamera = FindGO<GameCamera>("gamecamera");
	return true;
}

void Player::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_is2D = !m_is2D;
	}

	Move();
	Rotation();
	PlayAnimation();
	m_modelRender.SetScale(SCALE);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void Player::Move()
{
	m_moveSpeed = Vector3::Zero;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	if (m_is2D)
	{
		m_moveSpeed += right * (stickL.x * 480.0f);
		m_moveSpeed.z = 0.0f;
	}

	else
	{
		m_moveSpeed += right * (stickL.x * 480.0f);
		m_moveSpeed += forward * (stickL.y * 480.0f);
	}

	/* ジャンプ処理。
	 * 短ジャンプ: 低いジャンプ。
	 * 長ジャンプ: 高いジャンプ。
	 */
	if(m_Characon.IsOnGround())
	{
	//	m_moveSpeed.y = 0.0f;
		// ジャンプ
		if(g_pad[0]->IsTrigger(enButtonA))
		{
			m_moveSpeed.y = JUMP_INITIAL_SPEED;
		}
	} 
	
	else
	{
		if (m_moveSpeed.y > 0.0f) 
		{
			// 上昇中
			if (g_pad[0]->IsPress(enButtonA))
			{
				// ボタン押しっぱなしなら高く飛べる（重力を弱める）
				m_moveSpeed.y -= JUMP_HOLD_GRAVITY;
			}
			else 
			{
				// ボタンを離したらすぐに強めの重力
				m_moveSpeed.y -= GRAVITY;
			}
		}
		else
		{
			// 落下中はさらに重力を強めてズドンと落ちる
			m_moveSpeed.y -= FALL_GRAVITY;
		}
	}

	m_position = m_Characon.Execute(m_moveSpeed, 1.0f / 60.0f);
	if (m_is2D && m_gameCamera->IsInOrbitZone(m_position)) { m_position.z = 0.0f; }
	m_Characon.SetPosition(m_position);
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	Vector3 dir = m_moveSpeed;

	if (m_is2D) {
		dir.z = 0.0f;
	}

	if (fabsf(dir.x) >= 0.001f || fabsf(dir.z) >= 0.001f) {
		rot.SetRotationYFromDirectionXZ(dir);
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

void Player::SetAnimation()
{
	/* 待機アニメーション。*/
	FetchPlayAnimation(enAnimationClip_Idle,"playeridle", true);
	/* 歩きアニメーション。*/
	FetchPlayAnimation(enAnimationClip_Walk, "playerwalk", true);
	/* 走りアニメーション。*/
	FetchPlayAnimation(enAnimationClip_Run, "playerrun", true);
	/* ジャンプアニメーション。*/
	FetchPlayAnimation(enAnimationClip_Jump, "playerjump", false);
}