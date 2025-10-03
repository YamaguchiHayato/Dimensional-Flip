#include "stdafx.h"
#include "OrbitCameraStrategy.h"
#include "Src/Camera/GameCamera.h"
#include "graphics/Camera.h"
#include "Src/Actor/Character/Player.h"
#include <algorithm> 

using namespace nsK2EngineLow;

void OrbitCameraStrategy::Update(GameCamera* owner)
{
    Vector3 playerPos = owner->QueryPlayerPos();

    // 入力で回転要求
    if (g_pad[0]->IsTrigger(enButtonB)) {
        if (owner->IsInOrbitZone(playerPos)) {
            if (!m_isRotating) {
                BeginOrbitStep(owner);
            }
            else {
                ++m_rotateQueue;
            }
        }
    }

    float dt = g_gameTime->GetFrameDeltaTime();
    if (m_isRotating) {
        TickOrbit(owner, dt);
    }
    else {
        // 追従のみ
        if (g_camera3D) {
            g_camera3D->SetTarget(playerPos);
            g_camera3D->SetPosition(playerPos + owner->GetFollowOffset());
        }
    }
}

void OrbitCameraStrategy::BeginOrbitStep(GameCamera* owner)
{
    /* 初期オフセットを保存。*/
	m_initialOffset = owner->GetFollowOffset();

    if (g_camera3D) {
        m_initialOffset = g_camera3D->GetPosition() - owner->QueryPlayerPos();
        if (m_initialOffset.LengthSq() < 1e-6f) {
            m_initialOffset = Vector3(0, 50, -200);
        }
    }
    m_stepAngleRad = Math::DegToRad(m_step);
    m_duration = std::fabs(m_stepAngleRad / m_speed);
    m_isRotating = true;
    m_elapsed = 0.0f;
    m_currentAngleRad = 0.0f;
    m_targetAngleRad = m_stepAngleRad;
}

void OrbitCameraStrategy::TickOrbit(GameCamera* owner, float dt)
{
    Vector3 playerPos = owner->QueryPlayerPos();

    m_elapsed += dt;
    float t = std::min<float>(m_elapsed / m_duration, 1.0f);

    // イージング
    float u = t * t * (3.0f - 2.0f * t);
    float angleNow = m_targetAngleRad * u;

    Vector3 curOffset = m_initialOffset;
    Quaternion q;
	ApplyOrbit(curOffset, angleNow); // 現在の位置に現在の回転を適用してリセット

    Vector3 camPos = playerPos + curOffset;
    g_camera3D->SetTarget(playerPos);
    g_camera3D->SetPosition(camPos);

    if (m_elapsed >= m_duration) {
        owner->SetFollowOffset(curOffset);
        m_isRotating = false;
        m_elapsed = 0.0f;
        m_currentAngleRad = 0.0f;
        m_targetAngleRad = 0.0f;

        if (m_rotateQueue > 0) {
            --m_rotateQueue;
            BeginOrbitStep(owner);
        }
    }
    else {
        m_currentAngleRad = angleNow;
    }
}

void OrbitCameraStrategy::ApplyOrbit(Vector3& offset, float angleNow)
{
    Quaternion q;
    if (m_mode == OrbitMode::XZ) {
        q.SetRotationY(angleNow); // XZ 平面
    }
    else if (m_mode == OrbitMode::YZ) {
        q.SetRotationX(angleNow); // YZ 平面
    }
    q.Apply(offset);
}
