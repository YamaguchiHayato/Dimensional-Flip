#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
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
    
	animationClip_[enAnimationClip].Load(AnimationFilePath.c_str());
	animationClip_[enAnimationClip].SetLoopFlag(flag);
	return AnimationFilePath;
}

const std::string Player::FetchPlayerModel(const std::string& modelName, AnimationClip animationClip, EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag)
{
	// モデルをロード(tkmファイル名を打ち込む)。
	std::string  Player = PLAYER_MODEL + modelName + MODEL_FILE_EXTENSION;

	animationClip_[enAnimationClip].Load(Player.c_str());
	animationClip_[enAnimationClip].SetLoopFlag(flag);
	return Player;

}

bool Player::Start()
{
	SetAnimation();

	playerRender_.Init("Assets/modelData/unityChan.tkm", animationClip_, enAnimationClip_Num, enModelUpAxisY);

	playerCC_.Init(20.0f, 25.0f, playerPos_);

	return true;
}

void Player::Update()
{
	didJumpThisFrame_ = false;


	Move();
	Rotation();
	PlayAnimation();
	ManageState();
	playerRender_.SetScale(SCALE);
	playerRender_.SetPosition(playerPos_);
	playerRender_.Update();
}

void Player::Move()
{
	moveSpeed_.x = 0.0f;
	moveSpeed_.z = 0.0f;

	Vector3 stickL;

	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	right *= stickL.x * 480.0f;
	forward *= stickL.y * 0.0f;

	moveSpeed_ += right + forward;

	if (playerCC_.IsOnGround())
	{
		moveSpeed_.y = 0.0f;

		if (g_pad[0]->IsTrigger(enButtonA)) 
		{
			moveSpeed_.y = 350.0f;
			didJumpThisFrame_ = true;
		}
	}

	moveSpeed_.y -= GLAVITY;


	playerPos_ = playerCC_.Execute(moveSpeed_, 1.0f / 150.0f);


	playerCC_.SetPosition(playerPos_);
	playerRender_.SetPosition(playerPos_);
}

void Player::Rotation()
{
	Vector3 dir = moveSpeed_;

	if (fabsf(dir.x) >= 0.001f || fabsf(dir.z) >= 0.001f) {
		playerot_.SetRotationYFromDirectionXZ(dir);
		playerRender_.SetRotation(playerot_);
	}
}

//ステート管理
void Player::ManageState()
{
	//地面についていなかったら
	if (playerCC_.IsOnGround() == false)
	{
		//ステートを1にする
		playerState_ = 1;
		//ManageStateの処理を終わらせる
		return;
	}

	//地面に着地したら
	//x zの移動速度があったらスティックの入力
	if (fabsf(moveSpeed_.x) >= 0.001f || fabsf(moveSpeed_.z) >= 0.001f)
	{
		//ステートを2にする
		playerState_ = 2;
	}
	//何も入力しなかったら
	else
	{
		//ステートを0(待機状態)にする
		playerState_ = 0;
	}
}

void Player::PlayAnimation()
{
	//switch文
	switch (playerState_)
	{
		//待機状態だったら
	case 0:
		//待機アニメーションの再生
		playerRender_.PlayAnimation(enAnimationClip_Idle);
		break;
		//歩き状態だったら
	case 1:
		//ジャンプアニメーションを再生
		playerRender_.PlayAnimation(enAnimationClip_Jump);
		break;
		//ジャンプ中だったら
	case 2:
		playerRender_.PlayAnimation(enAnimationClip_Run);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	playerRender_.Draw(rc);
}

void Player::SetAnimation()
{
	FetchPlayAnimation(enAnimationClip_Idle,"idle", true);
	FetchPlayAnimation(enAnimationClip_Walk, "walk", true);
	FetchPlayAnimation(enAnimationClip_Run, "run", true);
	FetchPlayAnimation(enAnimationClip_Jump, "jump", false);
}
