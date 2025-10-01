#include "stdafx.h"
#include "Src/Camera/GameCamera.h"
#include "Src/Actor/Character/Player.h"
#include "OrbitCameraStrategy.h"
#include "FollowCamera.h"

bool GameCamera::Start()
{
	m_strategy = std::make_unique<OrbitCameraStrategy>();

    m_player = FindGO<Player>("player");

    if (g_camera3D) {
        Vector3 camPos = g_camera3D->GetPosition();
        m_followOffset = camPos - QueryPlayerPos();
    }
    return true;
}

void GameCamera::Update()
{
    if (m_strategy) {
        m_strategy->Update(this);
    }
}

Vector3 GameCamera::QueryPlayerPos() const
{
    if (m_player) return m_player->GetPosition();
    if (g_camera3D) return g_camera3D->GetTarget();
    return Vector3::Zero;
}

void GameCamera::AddOrbitZoneXZ(float minX, float maxX, float minZ, float maxZ)
{
    if (minX > maxX) std::swap(minX, maxX);
    if (minZ > maxZ) std::swap(minZ, maxZ);
    m_orbitZones.push_back(OrbitZoneXZ{ minX, maxX, minZ, maxZ });
}


void GameCamera::ClearOrbitZones()
{
    m_orbitZones.clear();
}

bool GameCamera::IsInOrbitZone(const Vector3& p) const
{
    if (m_orbitZones.empty()) return true;
    for (const auto& z : m_orbitZones) {
        if (p.x >= z.minX && p.x <= z.maxX &&
            p.z >= z.minZ && p.z <= z.maxZ) {
            return true;
        }
    }
    return false;
}
