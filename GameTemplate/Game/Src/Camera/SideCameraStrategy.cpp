#include "stdafx.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Core/CameraManager.h"
#include "Src/camera/FollowStrategy.h"

namespace
{
    // const Vector3 OFFSET(0.0f, 30.0f, -200.0f);
    const Vector3 OFFSET(0.0f, 0.0f, -800.0f);
    const float CAMERA_POS_OFFSET(80.0f);  //平行透視のため、視点と注視点の高さは同じ→このオフセットは視点と注視点どちらにも適用

}

SideCameraStrategy::SideCameraStrategy(Player* pPlayer) : pPlayer_(pPlayer)
{
    SetTargetRotationY(0.0f);
    // 画角の調整
    float aspect = 16.0 / 9.0f;
    auto zoomHeight = 240.0f;

    g_camera3D->SetWidth(zoomHeight * aspect);
    g_camera3D->SetHeight(zoomHeight);

    auto i=g_camera3D->GetNear();
    auto k=g_camera3D->GetFar();

    ////ニアクリップとファークリップの設定
    g_camera3D->SetNear(1.0f);
    // スカイキューブの大きさに合わせて調整
    g_camera3D->SetFar(5000.0f);
}

void SideCameraStrategy::Update(nsK2EngineLow::Camera* camera, const float deltaTime)
{
    if (!pPlayer_){return;}

    

    // 現在の回転を目標の回転に滑らかに近づける。*/
    float t = rotationSpeed_ * deltaTime;
    // t が 1.0fを超えないようにする。*/
    if (t > 1.0f){t = 1.0f;}
    // Slerp … 球面線形補完。(結果は currentRotation_ 自身に格納される) */
    currentRotation_.Slerp(t, currentRotation_, targetRotation_); //

    camera->SetRotation(currentRotation_);

    // --- 位置計算 ---
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = camera->GetPosition();

    Vector3 rotatedOffset = OFFSET;
    currentRotation_.Apply(rotatedOffset);

    // 理想の位置 = プレイヤーの位置 + 回転したオフセット
    const Vector3 idealPos = targetPos + rotatedOffset;

    // 
    const float followSpeed = 15.0f * deltaTime;
    // Lerp関数はFollowStrategyのstatic関数を使う
    Vector3 newPos = FollowStrategy::Lerp(followSpeed, currentCamPos, idealPos);
    newPos.y = targetPos.y + CAMERA_POS_OFFSET;
    camera->SetPosition(newPos);

    // --- 注視点計算 ---
    Vector3 lookAtTarget = targetPos;
    lookAtTarget.y += CAMERA_POS_OFFSET; 
    camera->SetTarget(lookAtTarget);
}
