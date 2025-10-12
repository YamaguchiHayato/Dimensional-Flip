#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/GameCamera.h"
#include "Src/UI/ScoreUI.h"
namespace
{
	const Vector3 SCALE(0.5f, 0.5f, 0.5f);

	// モデルにかかる重力。
	const float GLAVITY = 15.0f;


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
	/* アニメーションの設定。*/
	SetAnimation();

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClip, enAnimationClip_Num, enModelUpAxisY);

	m_Characon.Init(20.0f, 25.0f, m_position);

	m_gameCamera = FindGO<GameCamera>("gamecamera");
	return true;
}

void Player::Update()
{
	/* フレームの開始時にフラグをリセットする。*/
	didJumpThisFrame_ = false;

	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	m_is2D = !m_is2D;
	//}

	Move();
	Rotation();
	PlayAnimation();
	ManageState();
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

	if (m_Characon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;

		if (g_pad[0]->IsTrigger(enButtonA)) 
		{
			m_moveSpeed.y = 350.0f;
			didJumpThisFrame_ = true;
		}
	}

	m_moveSpeed.y -= GLAVITY;


	m_position = m_Characon.Execute(m_moveSpeed, 1.0f / 150.0f);


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
		m_rotation.SetRotationYFromDirectionXZ(dir);
		m_modelRender.SetRotation(m_rotation);
	}
}

//ステート管理
void Player::ManageState()
{
	//地面についていなかったら
	if (m_Characon.IsOnGround() == false)
	{
		//ステートを1にする
		m_playerState = 1;
		//ManageStateの処理を終わらせる
		return;
	}

	//地面に着地したら
	//x zの移動速度があったらスティックの入力
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//ステートを2にする
		m_playerState = 2;
	}
	//何も入力しなかったら
	else
	{
		//ステートを0(待機状態)にする
		m_playerState = 0;
	}
}

void Player::PlayAnimation()
{
	//switch文
	switch (m_playerState)
	{
		//待機状態だったら
	case 0:
		//待機アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//歩き状態だったら
	case 1:
		//ジャンプアニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
		//ジャンプ中だったら
	case 2:
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
	FetchPlayAnimation(enAnimationClip_Idle,"idle", true);
	/* 歩きアニメーション。*/
	FetchPlayAnimation(enAnimationClip_Walk, "walk", true);
	/* 走りアニメーション。*/
	FetchPlayAnimation(enAnimationClip_Run, "run", true);
	/* ジャンプアニメーション。*/
	FetchPlayAnimation(enAnimationClip_Jump, "jump", false);
}