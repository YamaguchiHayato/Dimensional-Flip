#include "stdafx.h"
#include "FollowCamera.h"
#include "Src/Camera/GameCamera.h"

void FollowCamera::Update(GameCamera* owner)
{
    if (!g_camera3D) return;

    nsK2EngineLow::Vector3 playerPos = owner->QueryPlayerPos();
    nsK2EngineLow::Vector3 camPos = playerPos + owner->GetFollowOffset();
    g_camera3D->SetTarget(playerPos);
    g_camera3D->SetPosition(camPos);

    owner->SetFollowOffset(camPos - playerPos);
}