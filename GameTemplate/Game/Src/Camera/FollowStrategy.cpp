#include "stdafx.h"
#include "FollowStrategy.h"
#include "Src/Actor/Character/Player.h"


namespace
{

    // 追従ライン（これ以下ならカメラ位置の基準は下がらない）
    const auto THRESHOLD_Y = 100.0f;
    const Vector3 OFFSET(0.0f, 60.0f, -25.0f);
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
    //if (!pPlayer_)
    //{
    //    return;
    //}

    //const Vector3 targetPos = pPlayer_->GetPlayerPos();
    //const Vector3 currentCamPos = g_camera3D->GetPosition();
    //const float stickX = g_pad[0]->GetRStickXF();
    //const float stickY = g_pad[0]->GetRStickYF() * -1.0f;

    //Vector3 idealOffset = OFFSET;
    //Quaternion rotY;
    //rotY.SetRotationDeg(Vector3::AxisY, 1.3f * stickX);
    //rotY.Apply(idealOffset);
    //targetRotation_.Apply(idealOffset);

    //// Cross関数はグローバル呼び出しでOKです
    //Vector3 axisX = Cross(Vector3::AxisY, idealOffset);
    //axisX.Normalize();
    //Quaternion rotX;
    //rotX.SetRotationDeg(axisX, 1.3f * stickY);
    //rotX.Apply(idealOffset);

    //const Vector3 idealPos = targetPos + idealOffset;
    //const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
    //const Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPos);

    //g_camera3D->SetPosition(newPos);

    //Vector3 lookAtPoint = targetPos;
    //lookAtPoint.y += 30.0f;
    //g_camera3D->SetTarget(lookAtPoint);



    if (!pPlayer_)
        return;

    // 取得。
    // 座標。
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = g_camera3D->GetPosition();
    // スティック
    const float stickX = g_pad[0]->GetRStickXF();
    const float stickY = g_pad[0]->GetRStickYF() * -1.0f;


    if (pPlayer_->GetCharacterController().IsOnGround())
        lastGroundY_ = targetPos.y;
    // ズレ防止用に初期化。
    if (lastGroundY_ == 0.0f && targetPos.y > 0.0f)
        lastGroundY_ = targetPos.y;


    Vector3 idealOffset = OFFSET;

    // Y軸回転
    Quaternion rotY;
    rotY.SetRotationDeg(Vector3::AxisY, 1.3f * stickX);
    rotY.Apply(idealOffset);
    targetRotation_.Apply(idealOffset); // 外部回転があれば適用


    // X軸回転
    Vector3 axisX = Cross(Vector3::AxisY, idealOffset);
    axisX.Normalize();
    Quaternion rotX;
    rotX.SetRotationDeg(axisX, 1.3f * stickY);
    rotX.Apply(idealOffset);


    // 高さ制限
    float desiredHeightOffset = idealOffset.y;
    Vector3 idealPosXZ = targetPos + idealOffset;


    // プレイヤーの座標を計算。
    float destY = CalculateThresholdY(targetPos.y, THRESHOLD_Y, desiredHeightOffset);


    // カメラ位置を補完。
    const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
    Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPosXZ);


    // Y座標の補完処理。
    float nextY = LerpFloat(currentCamPos.y, destY, 5.0f);


    // Y座標を適用
    newPos.y = nextY;
    g_camera3D->SetPosition(newPos);


    // 注視点 (プレイヤーの少し上を見る)
    Vector3 lookAtPoint = targetPos;
    // + する値は要調整。
    lookAtPoint.y =   lastGroundY_ + 30.0f;
    g_camera3D->SetTarget(lookAtPoint);
}
