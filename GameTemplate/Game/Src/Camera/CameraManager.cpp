#include "stdafx.h"
#include "CameraManager.h"
#include "Src/Actor/Character/Player.h"
#include "SideCameraStrategy.h"
#include "Src/camera/FollowStrategy.h"

bool CameraManager::Start()
{
	// プレイヤーを探す (戦略作成時に必要)
	pPlayer_ = FindGO<Player>("player");
	if (!pPlayer_) {
		K2_LOG("DimensionTrigger::Start() プレイヤーが見つかりません。\n");
		return false; 
	}

	// 初期戦略 (2Dモード) を設定
	pCameraStrategy_ = std::make_unique<SideCameraStrategy>(pPlayer_);
	pCameraStrategy_->Start();
	currentMode_ = CameraMode::mode2D;
	g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);

	return true;
}

void CameraManager::Update()
{
	if (pCameraStrategy_) {
		pCameraStrategy_->Update(g_camera3D, g_gameTime->GetFrameDeltaTime());
	}
}

void CameraManager::Request3DMode()
{
	// モードが既に3Dなら何もしない (任意)
	if (currentMode_ == CameraMode::mode3D) return;

	pCameraStrategy_ = std::make_unique<FollowStrategy>(pPlayer_);
	pCameraStrategy_->Start(); // 念のため初期化
	currentMode_ = CameraMode::mode3D;
	g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
}

void CameraManager::Request2DMode()
{
	// モードが既に2Dで、かつ回転していない場合 (任意)
	// if (m_currentMode == CameraMode::mode2D && /* 回転角度がほぼ0かチェック */ ) return;

	pCameraStrategy_ = std::make_unique<SideCameraStrategy>(pPlayer_);
	pCameraStrategy_->Start(); // 念のため初期化
	currentMode_ = CameraMode::mode2D;
	g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
}

void CameraManager::Request2DRotation(float targetAngleDegrees)
{
	// 現在のモードが2Dでない場合は何もしない
	if (currentMode_ != CameraMode::mode2D) return;

	// 現在の戦略が SideCameraStrategy かどうか確認
	SideCameraStrategy* sideStrategy = dynamic_cast<SideCameraStrategy*>(pCameraStrategy_.get());

	if (sideStrategy) {
		// 回転を指示
		sideStrategy->SetTargetRotationY(targetAngleDegrees);
	} 
	else 
	{
		// もし2Dモードなのに戦略が違う場合は、
		// SideCameraStrategy を作り直してから回転させる
		pCameraStrategy_ = std::make_unique<SideCameraStrategy>(pPlayer_);
		pCameraStrategy_->Start();
		sideStrategy = static_cast<SideCameraStrategy*>(pCameraStrategy_.get());
		sideStrategy->SetTargetRotationY(targetAngleDegrees);
	}
}
