#include "stdafx.h"
#include "FollowStrategy.h"

namespace {
    const Vector3 OFFSET(0.0f, 10.0f, -200.0f);
}

FollowStrategy::FollowStrategy(Player* pPlayer)
	: pPlayer_(pPlayer)
{
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

void FollowStrategy::Update(nsK2EngineLow::Camera* pCamera, const float deltaTime)
{
    if (!pPlayer_)
    {
        return;
    }

    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = pCamera->GetPosition();
    const float stickX = g_pad[0]->GetRStickXF();
    const float stickY = g_pad[0]->GetRStickYF() * -1.0f;
    Vector3 idealOffset = OFFSET;


    //todo for test
    Quaternion rotY = currentRotation_;
    rotY.SetRotationDeg(Vector3::AxisY, 1.3f * stickX);
    rotY.Apply(idealOffset);

    //currentRotation_ = rotY;

    // Cross関数はグローバル呼び出しでOKです
    Vector3 axisX = Cross(Vector3::AxisY, idealOffset);
    axisX.Normalize();
    Quaternion rotX;
    rotX.SetRotationDeg(axisX, 1.3f * stickY);
    rotX.Apply(idealOffset);

    targetRotation_.Apply(idealOffset);

    const Vector3 idealPos = targetPos + idealOffset;
    const float followSpeed = 15.0f * deltaTime;
    const Vector3 newPos = FollowStrategy::Lerp(followSpeed, currentCamPos, idealPos);

    pCamera->SetPosition(newPos);

    // 【修正点】注視点をプレイヤーの胴体（+30.0f）に修正
    Vector3 lookAtPoint = targetPos;
    lookAtPoint.y += 30.0f;
    pCamera->SetTarget(lookAtPoint);
}
