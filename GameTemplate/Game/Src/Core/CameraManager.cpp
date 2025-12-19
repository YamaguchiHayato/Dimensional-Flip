#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Core/StageManager.h"

// 戦略カメラクラス。
#include "Src/Camera/BossCameraStrategy.h"
#include "Src/Camera/FollowStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Camera/StageEXCameraStrategy.h"
#include "Src/Core/CameraManager.h"

// サウンド。
#include "Src/Core/SoundManager.h"

bool CameraManager::Start()
{
    pPlayer_ = FindGO<Player>("player");
    pBoss_ = FindGO<Boss>("boss");

    Request2DMode();
    //Request3DMode();
    return true;
}

void CameraManager::Update()
{
    if (!pCurrentCameraStrategy_)
    {
        return;
    }

    pCurrentCameraStrategy_->Update(g_camera3D, g_gameTime->GetFrameDeltaTime());

    ChangeCameraMode();
}

void CameraManager::Request2DMode()
{
    //// 戦略が存在する場合は、モードが同じでも生成する。
    /*if (currentMode_ == CameraMode::mode2_5D && pCurrentCameraStrategy_)
        return;*/
  
    pCurrentCameraStrategy_ = std::make_unique<SideCameraStrategy>(pPlayer_);
    pCurrentCameraStrategy_.get()->SetTargetRotationY(0.0f);

    if (pCurrentCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::mode2_5D;
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Ortho);
    }
}

void CameraManager::Request3DMode()
{
    pCurrentCameraStrategy_ = std::make_unique<FollowStrategy>(pPlayer_);
    pCurrentCameraStrategy_.get()->SetTargetRotationY(270.0f);
    if (pCurrentCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::mode3D;
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
    }
}

void CameraManager::RequestBossMode(float targetAngleDegrees)
{
    if (StageManager::GetInstance()->GetCurrentStageID() != StageID::sStageEX)
        return;

    pCurrentCameraStrategy_ = std::make_unique<app::camera::BossCameraStrategy>(pPlayer_);
    if (pCurrentCameraStrategy_->Start())
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

    pCurrentCameraStrategy_ = std::make_unique<app::camera::StageEXCameraStrategy>(pPlayer_);

    if (pCurrentCameraStrategy_->Start())
    {
        currentMode_ = CameraMode::modeStageEX;
        // 俯瞰視点なのでパースペクティブ（遠近感あり）
        g_camera3D->SetUpdateProjMatrixFunc(nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
    }
}

void CameraManager::ChangeCameraMode()
{
    if (g_pad[0]->IsTrigger(enButtonB) && pPlayer_->GetInTriggerArea())
    {

        if (!pCurrentCameraStrategy_)
            return;

        
        float currentTargetAngle = pCurrentCameraStrategy_->GetTargetRotationY();
        app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Flip);

        // トグル操作。
        if (currentMode_!=CameraMode::mode2_5D)
        {
            Request2DMode();
            return;
        }
        if(currentMode_ != CameraMode::mode3D)
        {
            Request3DMode();
            return;
        }
    }
}
