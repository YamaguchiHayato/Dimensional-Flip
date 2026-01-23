#include "stdafx.h"
#include "Src/Core/CameraManager.h"

#include "Src/Actor/Character/Player/Player.h"

// 戦略カメラクラス。
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Camera/FollowStrategy.h"
#include "Src/Camera/BossCameraStrategy.h"


namespace
{
    const float INIT_CAMERA_ANGLE_2D = 0.0f;
    const float INIT_CAMERA_ANGLE_3D = 90.0f;
}


bool CameraManager::Start()
{
    // プレイヤーの取得。
    pPlayer_ = FindGO<Player>("player");

    // ゲームスタート時は2Dモードに設定
    Request2DMode();

    return true;
}


void CameraManager::Update()
{
    if (!pCameraStrategy_)
        return;

    // カメラ戦略の更新
    pCameraStrategy_->Update();
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


template <typename CameraType>
void CameraManager::RequestCameraMode(const float angle, CameraMode cameraMode, const nsK2EngineLow::Camera::EnUpdateProjMatrixFunc mode)
{
    // 既存のカメラ戦略を停止・破棄
    pCameraStrategy_ = std::make_unique<CameraType>(pPlayer_);
    pCameraStrategy_.get()->SetTargetRotationY(angle);

    // カメラ制限が設定されていれば適用
    if (isLimitSet_)
    {
        if (auto* side = dynamic_cast<SideCameraStrategy*>(pCameraStrategy_.get()))
        {
            side->SetCameraLimit(limitMin_, limitMax_);
        }
        else if (auto* follow = dynamic_cast<FollowStrategy*>(pCameraStrategy_.get()))
        {
            follow->SetCameraLimit(limitMin_, limitMax_);
        }
    }

    // カメラ開始
    if (pCameraStrategy_->Start())
    {
        currentMode_ = cameraMode;
        g_camera3D->SetUpdateProjMatrixFunc(mode);
    }
}




void CameraManager::SetCameraRange(const Vector3& min, const Vector3& max)
{
    limitMin_ = min;
    limitMax_ = max;
    isLimitSet_ = true;

    // 現在稼働中のカメラにも適用
    if (pCameraStrategy_)
    {
        if (auto* side = dynamic_cast<SideCameraStrategy*>(pCameraStrategy_.get()))
        {
            side->SetCameraLimit(min, max);
        }
        else if (auto* follow = dynamic_cast<FollowStrategy*>(pCameraStrategy_.get()))
        {
            follow->SetCameraLimit(min, max);
        }
    }
}
