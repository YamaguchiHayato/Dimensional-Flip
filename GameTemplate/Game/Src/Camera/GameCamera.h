#pragma once
#include "ICameraStrategy.h"

/*
 * @ struct OrbitZoneXZ 
 * 
 * XZ平面上の視点回転許可領域を表す構造体。
 */
struct OrbitZoneXZ
{
    float minX, 
          maxX, 
          minZ, 
          maxZ; 
};


/*
 * @ struct OrbitZoneYZ 
 * 
 * YZ平面上の視点回転許可領域を表す構造体。
 */ 
struct  OrbitZoneYZ
{
    float minY,
          maxY,
          minZ,
          maxZ;
};

class Player;
class GameCamera : public IGameObject
{
public:
    GameCamera() = default;
    ~GameCamera() = default;

    bool Start() override;
    void Update() override;

    void SetStrategy(std::unique_ptr<ICameraStrategy> strategy) {
        m_strategy = std::move(strategy);
    }

    // --- 共通ユーティリティ ---
    nsK2EngineLow::Vector3 QueryPlayerPos() const;
    void SetFollowOffset(const nsK2EngineLow::Vector3& ofs) { m_followOffset = ofs; }
    const nsK2EngineLow::Vector3& GetFollowOffset() const { return m_followOffset; }

    bool IsInOrbitZone(const nsK2EngineLow::Vector3& p) const;
    void AddOrbitZoneXZ(float minX, float maxX, float minZ, float maxZ);
    void AddOrbitZoneYZ(float minY, float maxY, float minZ, float maxZ);

    void ClearOrbitZones();

    /*
     * @ unique_ptr をそのまま返すとコピーできないので、
     * .get() を使って「参照専用の生ポインタ」を返す。
     */
    void SetStrategyType(std::unique_ptr<ICameraStrategy> strategy) {
        m_strategy = std::move(strategy);
	}

	ICameraStrategy* GetStrategy() const { return m_strategy.get(); }
private:
    std::unique_ptr<ICameraStrategy> m_strategy;
    Player* m_player = nullptr;
    nsK2EngineLow::Vector3 m_playerPos{ 0,0,0 };
    nsK2EngineLow::Vector3 m_followOffset{ 0,0,-200 };


     
    std::vector<OrbitZoneXZ> m_zoneXZ;
    std::vector<OrbitZoneYZ> m_zoneYZ;
};
