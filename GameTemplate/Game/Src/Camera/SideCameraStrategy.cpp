#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Camera/SideCameraStrategy.h"

namespace
{
    const Vector3 OFFSET = {0.0f, 0.0f, -200.0f};
    const float FOLLOW_START_POSITION = 9.0f; // 追従ライン
    const float FOOT_TO_CENTER_OFFSET = 1.0f;
} 

SideCameraStrategy::SideCameraStrategy(Player* pPlayer)
{
    // 視点のアスペクト比を設定
    InitCameraAspect();

    // todo for test pPlayerを基底クラスに追加と代入出来るのか試す
    pPlayer_ = pPlayer;
    SetTargetRotationY(0.0f);

    // 近平面と遠平面の設定
    g_camera3D->SetNear(1.0f);
    // スカイキューブの大きさに合わせて調整
    g_camera3D->SetFar(5000.0f);
}

void SideCameraStrategy::Update()
{
    if (!pPlayer_)
        return;

    //// 視点の計算 ////////////////////////////////////////////////
    // プレイヤーの位置の取得
    const Vector3 targetPos = pPlayer_->GetPlayerPos();

    // オフセットの適用(視点を手前に引く)
    Vector3 offsetViewPos = targetPos + OFFSET;

    offsetViewPos.y = targetPos.y + FOOT_TO_CENTER_OFFSET;
    if (targetPos.y <= FOLLOW_START_POSITION - FOOT_TO_CENTER_OFFSET)
    {
        offsetViewPos.y = FOLLOW_START_POSITION;
    }
    g_camera3D->SetPosition(offsetViewPos);

    //// 注視点の計算 //////////////////////////////////////////////
    Vector3 lookAtTarget = targetPos;
    lookAtTarget.y = offsetViewPos.y; // 水平を見る
    g_camera3D->SetTarget(lookAtTarget);
}
