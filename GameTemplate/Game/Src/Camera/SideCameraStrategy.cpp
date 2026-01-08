#include "stdafx.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const Vector3 OFFSET(0.0f, 0.0f, -200.0f);
    const float HEIGHT_OFFSET = 3.0f; // 注視点と視点を一律で上げる
    const float ROTATION_SPEED = 5.0f;
    // 追従ライン。
    const auto THRESHOLD_Y = 5.0f;

}

SideCameraStrategy::SideCameraStrategy(Player* pPlayer)
{
    // 視点のアスペクト比をせていする。
    SetCameraAspect();

    // todo for test pPlayerを基底クラスに追加と代入出来るのか試す
    pPlayer_ = pPlayer;
	SetTargetRotationY(0.0f);


    //ニアクリップとファークリップの設定
    g_camera3D->SetNear(1.0f);
    // スカイキューブの大きさに合わせて調整
    g_camera3D->SetFar(700.0f);
}


void SideCameraStrategy::Update()
{
    if (!pPlayer_)
    {
        return;
    }

    // 回転
    float t = 1.0f * g_gameTime->GetFrameDeltaTime();
    if (t > 1.0f)
        t = 1.0f;
    currentRotation_.Slerp(t, currentRotation_, targetRotation_);
    g_camera3D->SetRotation(currentRotation_);

    // 位置計算
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = g_camera3D->GetPosition();

    float destY = CalculateThresholdY(targetPos.y, THRESHOLD_Y, HEIGHT_OFFSET);

    float nextY = LerpFloat(currentCamPos.y, destY, 5.0f);

    // X/Z計算
    Vector3 rotatedOffset = OFFSET;
    currentRotation_.Apply(rotatedOffset);
    const Vector3 idealPos = targetPos + rotatedOffset;

    const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
    Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPos);

    // Y座標適用
    newPos.y = nextY;

    // 注視点
    Vector3 lookAtTarget = targetPos;
    lookAtTarget.y = newPos.y; // 水平を見る

    g_camera3D->SetPosition(newPos);
    currentPos_ = newPos;
    g_camera3D->SetTarget(lookAtTarget);
}
