#include "stdafx.h"
#include "FollowStrategy.h"
#include "Src/Actor/Character/Player.h"


namespace
{
    const Vector3 OFFSET(0.0f, 20.0f, -200.0f);
}


FollowStrategy::FollowStrategy(Player* pPlayer)
{
    pPlayer_ = pPlayer;
    SetTargetRotationY(0.0f);
}


bool FollowStrategy::Start()
{
	////ニアクリップとファークリップの設定
	g_camera3D->SetNear(10.0f);         
	// スカイキューブの大きさに合わせて調整
	g_camera3D->SetFar(700.0f);      

	return true;
}

void FollowStrategy::Update()
{
	if (!pPlayer_) { return; }

	const Vector3 targetPos = pPlayer_->GetPlayerPos();
	const Vector3 currentCamPos = g_camera3D->GetPosition();
	const float stickX = g_pad[0]->GetRStickXF();
	const float stickY = g_pad[0]->GetRStickYF() * -1.0f;

    Vector3 idealOffset = OFFSET;
	Quaternion rotY;
	rotY.SetRotationDeg(Vector3::AxisY, 1.3f * stickX);
	rotY.Apply(idealOffset);
    targetRotation_.Apply(idealOffset);

	// Cross関数はグローバル呼び出しでOKです
	Vector3 axisX = Cross(Vector3::AxisY, idealOffset);
	axisX.Normalize();
	Quaternion rotX;
	rotX.SetRotationDeg(axisX, 1.3f * stickY);
	rotX.Apply(idealOffset);

	const Vector3 idealPos = targetPos + idealOffset;
	const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
	const Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPos);

	g_camera3D->SetPosition(newPos);

	// 【修正点】注視点をプレイヤーの胴体（+30.0f）に修正
	Vector3 lookAtPoint = targetPos;
	lookAtPoint.y += 30.0f;
    g_camera3D->SetTarget(lookAtPoint);
}
