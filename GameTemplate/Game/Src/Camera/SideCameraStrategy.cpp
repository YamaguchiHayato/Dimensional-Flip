#include "stdafx.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Actor/Character/Player.h"

namespace
{
    const Vector3 OFFSET(0.0f, 0.0f, -200.0f);
    const float HEIGHT_OFFSET = 50.0f; // 注視点と視点を一律で上げる
    const float ROTATION_SPEED = 5.0f;
}

SideCameraStrategy::SideCameraStrategy(Player* pPlayer)
{

    auto aspect = 16.0 / 9.0f;
    auto zoomHeight = 220.0f;

    g_camera3D->SetHeight(zoomHeight);         
    g_camera3D->SetWidth(zoomHeight * aspect);


    // todo for test pPlayerを基底クラスに追加と代入出来るのか試す
    pPlayer_ = pPlayer;
	SetTargetRotationY(0.0f);

    //ニアクリップとファークリップの設定
    g_camera3D->SetNear(1.0f);
    // スカイキューブの大きさに合わせて調整
    g_camera3D->SetFar(5000.0f);
}


void SideCameraStrategy::Update()
{
    if (!pPlayer_){ return; }        

    // 現在の回転を目標の回転んに滑らかに近づける。*/
    float t = ROTATION_SPEED * g_gameTime->GetFrameDeltaTime();

    // t が 1.0fを超えないようにする。*/
    if (t > 1.0f)
        t = 1.0f;

    // Slerp … 球面線形補完。(結果は currentRotation_ 自身に格納される) */
    currentRotation_.Slerp(t, currentRotation_, targetRotation_); //
    g_camera3D->SetRotation(currentRotation_);

    // --- 位置計算 ---
    const Vector3 targetPos = pPlayer_->GetPlayerPos();
    const Vector3 currentCamPos = g_camera3D->GetPosition();

    Vector3 rotatedOffset = OFFSET;
    currentRotation_.Apply(rotatedOffset);

    // 理想の位置 = プレイヤーの位置 + 回転したオフセット
    const Vector3 idealPos = targetPos + rotatedOffset;

    const float followSpeed = 15.0f * g_gameTime->GetFrameDeltaTime();
    // Lerp関数はFollowStrategyのstatic関数を使う
    Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPos);
    newPos.y = targetPos.y + HEIGHT_OFFSET;
    g_camera3D->SetPosition(newPos);

    // --- 注視点計算 ---
    Vector3 lookAtTarget = targetPos;
    lookAtTarget.y += HEIGHT_OFFSET;
    g_camera3D->SetTarget(lookAtTarget);
}

