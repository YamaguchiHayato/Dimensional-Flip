#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Camera/CameraManager.h"
#include "Src/Actor/Character/Player.h"
#include "SideCameraStrategy.h"
#include "Src/camera/FollowStrategy.h"
#include "BossCameraStrategy.h"

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
	currentMode_ = CameraMode::mode2_5D;
	g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);


    pBoss_ = FindGO<Boss>("boss");
	return true;
}

void CameraManager::Update()
{

	if (pCameraStrategy_) 
		pCameraStrategy_->Update(g_camera3D, g_gameTime->GetFrameDeltaTime());
}

void CameraManager::Request2DMode()
{
	pCameraStrategy_ = std::make_unique<SideCameraStrategy>(pPlayer_);
	pCameraStrategy_->Start(); // 念のため初期化
	currentMode_ = CameraMode::mode2_5D;
	g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
}

void CameraManager::Request3DModeRot(float targetAngleDegrees)
{
	// 現在のモードが2Dでない場合は何もしない
	if (currentMode_ != CameraMode::mode2_5D) return;
    
	// 現在の戦略が SideCameraStrategy かどうか確認
	SideCameraStrategy* sideStrategy = dynamic_cast<SideCameraStrategy*>(pCameraStrategy_.get());

	if (sideStrategy) 
		sideStrategy->SetTargetRotationY(targetAngleDegrees);

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

void CameraManager::Request3DMode()
{
    if (currentMode_ == CameraMode::mode3D)
        return;

    pCameraStrategy_ = std::make_unique<FollowStrategy>(pPlayer_); 
    pCameraStrategy_->Start();
    currentMode_ = CameraMode::mode3D;
    g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
}

void CameraManager::RequestBossMode()
{
    if (!pBoss_)
        return;

    // 1. 強制的に反時計回り(90度)の視点をカメラに適用
    SideCameraStrategy* sideStrategy = dynamic_cast<SideCameraStrategy*>(pCameraStrategy_.get());

    if (sideStrategy)
    {
        // SideCameraStrategyの回転目標を90度に設定（アニメーション設定）
        sideStrategy->SetTargetRotationY(90.0f);

        // アニメーションを待たずに、カメラの回転を即座に90度に設定（視点の強制変更）
        nsK2EngineLow::Quaternion instantRot;
        instantRot.SetRotationDeg(Vector3::AxisY, 90.0f);
        g_camera3D->SetRotation(instantRot);
    }

    // 2. BossCameraStrategyに切り替える
    pCameraStrategy_ = std::make_unique<BossCameraStrategy>();
    pCameraStrategy_->Start();
    currentMode_ = CameraMode::mode3D; // 3Dモードへ移行

    // 3. 奥行きを持たせるため、透視投影 (Perspective) に変更
    // ★最も重要な修正点：OrthoからPerspectiveに変更
    g_camera3D->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
}
