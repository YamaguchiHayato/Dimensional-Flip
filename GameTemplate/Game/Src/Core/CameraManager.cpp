#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"

#include "Src/Core/StageManager.h"

// 戦略カメラクラス。
#include "Src/Core/CameraManager.h"
#include "Src/Camera/BossCameraStrategy.h"
#include "Src/Camera/FollowStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Camera/StageEXCameraStrategy.h" 


bool CameraManager::Start()
{
    pPlayer_ = FindGO<Player>("player");
    pBoss_ = FindGO<Boss>("boss");

    Request2DMode();
    return true;
}


void CameraManager::Update()
{
    if (pCameraStrategy_)
        pCameraStrategy_->Update(g_camera3D, g_gameTime->GetFrameDeltaTime());
}


void CameraManager::Request2DMode()
{
    // 戦略が存在する場合は、モードが同じでも生成する。
    if (currentMode_ == CameraMode::mode2_5D && pCameraStrategy_)
        return;

    pCameraStrategy_ = std::make_unique<SideCameraStrategy>(pPlayer_);
    if (pCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::mode2_5D;
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Ortho);
    }
}


void CameraManager::Request3DMode()
{
    if (currentMode_ == CameraMode::mode3D)
        return;

    pCameraStrategy_ = std::make_unique<FollowStrategy>(pPlayer_);
    if (pCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::mode3D;
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
    }
}


void CameraManager::RequestBossMode(float targetAngleDegrees)
{
    if (StageManager::GetInstance()->GetCurrentStageID() != StageID::sStageEX)
        return;

    pCameraStrategy_ = std::make_unique<app::camera::BossCameraStrategy>(pPlayer_);
    if (pCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::modeBoss;
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
    }
}


void CameraManager::RequestStageExMode()
{
    if (StageManager::GetInstance()->GetCurrentStageID() != StageID::sStageEX)
        return;

    if (currentMode_ == CameraMode::modeStageEX)
        return;

    pCameraStrategy_ = std::make_unique<app::camera::StageEXCameraStrategy>(pPlayer_);

    if (pCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::modeStageEX;
        // 俯瞰視点なのでパースペクティブ（遠近感あり）
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
    }
}


void CameraManager::Request3DModeRot(float targetAngleDegrees)
{
    // 1 ～ 3ステージにて適応。
    if (currentMode_ == CameraMode::mode2_5D)
    {
        auto* strategy = dynamic_cast<SideCameraStrategy*>(pCameraStrategy_.get());
        if (strategy)
            strategy->SetTargetRotationY(targetAngleDegrees);
    }

    // ボスステージの場合、適応。
    else if (currentMode_ == CameraMode::modeStageEX)
    {
        auto* strategy = dynamic_cast<app::camera::StageEXCameraStrategy*>(pCameraStrategy_.get());
        if (strategy)
            strategy->SetTargetRotationY(targetAngleDegrees);
    }
}
