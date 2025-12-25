#include "stdafx.h"
#include "Src/Core/CameraManager.h"

#include "Src/Actor/Character/Player.h"

// 戦略カメラクラス。
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Camera/FollowStrategy.h"
#include "Src/Camera/BossCameraStrategy.h"
#include "Src/Camera/StageEXCameraStrategy.h" 


namespace
{
    const float INIT_CAMERA_ANGLE_2D = 0.0f;
    const float INIT_CAMERA_ANGLE_3D = 90.0f;
}


bool CameraManager::Start()
{
    pPlayer_ = FindGO<Player>("player");
    // ゲームスタート時は2Dモードに設定
    Request2DMode();

    return true;
}


void CameraManager::Update()
{
    if (!pCameraStrategy_){return;}

    pCameraStrategy_->Update();

    // カメラモードの切り替え要求をチェックs
//    ChangeCamera();
}


void CameraManager::Request2DMode()
{
    RequestCameraMode<SideCameraStrategy>(INIT_CAMERA_ANGLE_2D, CameraMode::mode2D, nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Ortho);
}


void CameraManager::Request3DMode()
{
    RequestCameraMode<FollowStrategy>(INIT_CAMERA_ANGLE_3D, CameraMode::mode3D, nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
}


void CameraManager::RequestBossMode(float targetAngleDegrees)
{
    RequestCameraMode<app::camera::BossCameraStrategy>(targetAngleDegrees, CameraMode::modeBoss, nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
}


void CameraManager::RequestStageExMode()
{
    RequestCameraMode<app::camera::StageEXCameraStrategy>(0.0f, CameraMode::modeStageEX, nsK2EngineLow::Camera::enUpdateProjMatrixFunc_Perspective);
}


template <typename CameraType>
void CameraManager::RequestCameraMode(const float angle, CameraMode cameraMode, const nsK2EngineLow::Camera::EnUpdateProjMatrixFunc mode)
{
    pCameraStrategy_ = std::make_unique<CameraType>(pPlayer_);
    pCameraStrategy_.get()->SetTargetRotationY(angle);
    if (pCameraStrategy_->Start())
    {
        currentMode_ = cameraMode;
        g_camera3D->SetUpdateProjMatrixFunc(mode);
    }
}


void CameraManager::ChangeCamera()
{
    // Bボタンが押されたらカメラモードを切り替え
    // 2Dモードなら3Dモードへ、3Dモードなら2Dモードへ
    if (g_pad[0]->IsTrigger(enButtonB) && currentMode_ == CameraMode::mode2D)
    {
        Request3DMode();
        return;
    }
    else if (g_pad[0]->IsTrigger(enButtonB) && currentMode_ == CameraMode::mode3D)
    {
        Request2DMode();
        return;
    }
}
