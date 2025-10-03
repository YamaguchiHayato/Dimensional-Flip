#include "stdafx.h"
#include "Src/Camera/GameCamera.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/OrbitCameraStrategy.h"
#include "Src/Camera/FollowCamera.h"

bool GameCamera::Start()
{
    //	m_strategy = std::make_unique<OrbitCameraStrategy>();
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
    m_zoneXZ.push_back(OrbitZoneXZ{ minX, maxX, minZ, maxZ });
}

void GameCamera::AddOrbitZoneYZ(float minY, float maxY, float minZ, float maxZ)
{
    if (minY > maxY) std::swap(minY, maxY);
    if (minZ > maxZ) std::swap(minZ, maxZ);
    m_zoneYZ.push_back(OrbitZoneYZ{ minY, maxY, minZ, maxZ });
}


void GameCamera::ClearOrbitZones()
{
    m_zoneXZ.clear();
    m_zoneYZ.clear();
}

bool GameCamera::IsInOrbitZone(const Vector3& p) const
{
    /* 双方に空の状態なら「制限なし」とする。*/
    if (m_zoneXZ.empty() && m_zoneYZ.empty()) return true;

    /* XZ方向の判定。*/
    for (const auto& z : m_zoneXZ) {
        // ★★★ このデバッグコードで全てが分かります ★★★
        char str[256];
        sprintf_s(str, "  CHECKING vs ZONE -> X:[%.1f to %.1f], Z:[%.1f to %.1f]\n", z.minX, z.maxX, z.minZ, z.maxZ);
        OutputDebugStringA(str);

        if (p.x >= z.minX && p.x <= z.maxX &&
            p.z >= z.minZ && p.z <= z.maxZ) {
            OutputDebugStringA("  --> RESULT: IN ZONE (TRUE)\n");
            return true;
        }
    }

    /* YZ方向の判定。*/
    for (const auto& z : m_zoneYZ) {
        // ★★★ YZ判定にもデバッグコードを追加 ★★★
        char str[256];
        sprintf_s(str, "  CHECKING vs ZONE -> Y:[%.1f to %.1f], Z:[%.1f to %.1f]\n", z.minY, z.maxY, z.minZ, z.maxZ);
        OutputDebugStringA(str);
        if (p.y >= z.minY && p.y <= z.maxY &&
            p.z >= z.minZ && p.z <= z.maxZ) {
            OutputDebugStringA("  --> RESULT: IN ZONE (TRUE)\n");
            return true;
        }
    }

    OutputDebugStringA("  --> RESULT: OUT OF ALL ZONES (FALSE)\n");
    return false;
}