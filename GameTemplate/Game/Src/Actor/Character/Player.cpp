#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/UI/ScoreUI.h"
#include "Src/Camera/CameraManager.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Camera/ICameraStrategy.h"

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

	render_.Init("Assets/modelData/unityChan.tkm", animationClip_, EnAnimationClip::Num, enModelUpAxisY);

	charaCon_.Init(20.0f, 25.0f, pos_);

    
	return true;
}

void Player::Update()
{
    if (isPaused_)
    {
        return;
    }
	didJumpThisFrame_ = false;


	Move();
    Action();
	Rotation();
	PlayAnimation();
	ManageState();
	render_.SetScale(SCALE);
	render_.SetPosition(pos_);
	render_.Update();
}

void Player::Action()
{
	if (!pCameraManager_) { return; } 
	CameraMode currentMode = pCameraManager_->GetCurrentCameraMode();

	bool isInTriggerArea = GetInTriggerArea();

    // Yボタンで3Dモード要求 (エリア内 かつ 2Dモード)
    if (g_pad[0]->IsTrigger(enButtonY) && isInTriggerArea  && currentMode == CameraMode::mode2D ) {
		pCameraManager_->Request3DMode();	
	}
	// Xボタンで元の2Dモード要求 (エリア内 かつ 3Dモード)
	else if (g_pad[0]->IsTrigger(enButtonX) && isInTriggerArea && currentMode == CameraMode::mode3D) {
		pCameraManager_->Request2DMode();
	}
	// Bボタンで2Dカメラ回転トグル (エリア内 かつ 2Dモード)
	else if(g_pad[0]->IsTrigger(enButtonB) && isInTriggerArea && currentMode == CameraMode::mode2D) {
		ICameraStrategy* currentStrategy = pCameraManager_->GetCurrentStrategy();
		SideCameraStrategy* sideStrategy = dynamic_cast<SideCameraStrategy*>(currentStrategy);
		if (sideStrategy) {
			float currentTargetAngle = sideStrategy->GetTargetRotationY(); 
            
            // トグル操作。
            if (fabsf(currentTargetAngle - 90.0f) < 1.0f) 
				pCameraManager_->Request2DRotation(0.0f);
			else 
				pCameraManager_->Request2DRotation(90.0f);
		}
	}
}

void Player::Move()
{
    moveSpeed_.x = 0.0f;
	moveSpeed_.z = 0.0f;

	Vector3 stickL;

	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

    // 常に g_camera3D (現在のメインカメラ) を基準にする
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

    // 左右の移動 (stickL.x) は、常にカメラの「右」方向ベクトルで計算
	right *= stickL.x * 480.0f;

    // カメラのモードを取得
    CameraMode currentMode = CameraMode::mode2D; 
    if (pCameraManager_) {
        currentMode = pCameraManager_->GetCurrentCameraMode();
    }

    // 前後の移動 (stickL.y) は3Dモードの時だけ有効にする
    if (currentMode == CameraMode::mode3D)
    {
        forward *= stickL.y * 480.0f;
    }
    else
    {
        forward *= 0.0f; 
    }
    
	moveSpeed_ += right + forward;

	if (charaCon_.IsOnGround())
	{
		moveSpeed_.y = 0.0f;

		if (g_pad[0]->IsTrigger(enButtonA)) 
		{
			moveSpeed_.y = 350.0f;
			didJumpThisFrame_ = true;
		}
	}

	moveSpeed_.y -= GLAVITY;


	pos_ = charaCon_.Execute(moveSpeed_, 1.0f / 150.0f);


	charaCon_.SetPosition(pos_);
	render_.SetPosition(pos_);
}

void Player::Rotation()
{
	Vector3 dir = moveSpeed_;

	if (fabsf(dir.x) >= 0.001f || fabsf(dir.z) >= 0.001f) {
		rot_.SetRotationYFromDirectionXZ(dir);
		render_.SetRotation(rot_);
	}
}

//ステート管理
void Player::ManageState()
{
	//地面についていなかったら
	if (charaCon_.IsOnGround() == false)
	{
		//ステートを1にする
		state_ = PlayerState::sJump;
		//ManageStateの処理を終わらせる
		return;
	}

	//地面に着地したら
	//x zの移動速度があったらスティックの入力
	if (fabsf(moveSpeed_.x) >= 0.001f || fabsf(moveSpeed_.z) >= 0.001f)
	{
		//ステートを2にする
		state_ = PlayerState::sRun;
	}
	//何も入力しなかったら
	else
	{
		//ステートを0(待機状態)にする
        state_ = PlayerState::sIdle;
	}
}

void Player::PlayAnimation()
{
	//switch文
	switch (state_)
	{
		//待機状態だったら
    case PlayerState::sIdle:
		//待機アニメーションの再生
		render_.PlayAnimation(EnAnimationClip::Idle);
		break;
		//歩き状態だったら
    case PlayerState::sJump:
		//ジャンプアニメーションを再生
		render_.PlayAnimation(EnAnimationClip::Jump);
		break;
		//ジャンプ中だったら
    case PlayerState::sRun:
		render_.PlayAnimation(EnAnimationClip::Run);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	render_.Draw(rc);
}

void Player::SetAnimation()
{
    // 待機アニメーション。
	FetchPlayAnimation(EnAnimationClip::Idle,"idle", true);
    // 走りアニメーション。
	FetchPlayAnimation(EnAnimationClip::Run, "run", true);
    // ジャンプアニメーション。
	FetchPlayAnimation(EnAnimationClip::Jump, "jump", false);
}
