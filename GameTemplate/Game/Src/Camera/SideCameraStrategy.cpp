#include "stdafx.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Actor/Character/Player.h"
#include "Src/camera/FollowStrategy.h"
#include "Src/Camera/CameraManager.h"

SideCameraStrategy::SideCameraStrategy(Player* pPlayer)
	: pPlayer_(pPlayer)
{
	SetTargetRotationY(0.0f);
	currentRotation_ = targetRotation_;
}

void SideCameraStrategy::Update(nsK2EngineLow::Camera* camera, const float deltaTime)
{
    if (!pPlayer_) return;

	// 現在の回転を目標の回転んに滑らかに近づける。*/
	float t =  rotationSpeed_ * deltaTime;
	// t が 1.0fを超えないようにする。*/
	if (t > 1.0f) t = 1.0f;
	// Slerp … 球面線形補完。(結果は currentRotation_ 自身に格納される) */
	currentRotation_.Slerp(t, currentRotation_, targetRotation_); //

	// --- 位置計算 ---
	const Vector3 targetPos = pPlayer_->GetPlayerPos();
	const Vector3 currentCamPos = camera->GetPosition();

	Vector3 rotatedOffset = baseOffset_; 
	currentRotation_.Apply(rotatedOffset); 

	// 理想の位置 = プレイヤーの位置 + 回転したオフセット
	const Vector3 idealPos = targetPos + rotatedOffset;
	
	const float followSpeed = 15.0f * deltaTime;
	// Lerp関数はFollowStrategyのstatic関数を使う
	const Vector3 newPos = FollowStrategy::Lerp(followSpeed, currentCamPos, idealPos);

	camera->SetPosition(newPos);

	// --- 注視点計算 ---
	Vector3 lookAtTarget = targetPos; 
	lookAtTarget.y += 30.0f; // プレイヤーの少し上を見る 	
	camera->SetTarget(lookAtTarget);
}
