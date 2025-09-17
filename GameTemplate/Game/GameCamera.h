#pragma once
#include "k2EngineLowPreCompile.h"
#include <functional>
#include <Vector>

class Player;
class GameCamera : public IGameObject
{
public:
    GameCamera() = default;
    ~GameCamera() = default;

    bool Start();
    void Update();
    void CameraUpdate();

    void SetPlayerGetter(const std::function<nsK2EngineLow::Vector3(void)>& getter) { m_getPlayerPos = getter; }
    void SetOrbitStepDeg(float degCW);
    void SetOrbitDuration(float seconds);
    void SetTarget(Player* player) { m_player = player; }
    void SetToCameraPos(const Vector3& toCameraPos)
    {
        m_toCameraPos = toCameraPos;
        m_toCameraPosmultiplier = double(m_toCameraPos.z) / double(-500.0f);
    }

    void AddOrbitZoneXZ(float minX, float maxX, float minZ, float maxZ);
    void ClearOrbitZones();
    bool IsInOrbitZone(const nsK2EngineLow::Vector3& p) const;

private:
    nsK2EngineLow::Vector3 QueryPlayerPos() const;
    void BeginOrbitStepCW();
    void TickOrbit(float dt);

    // --- ここを変更：std::clamp を使わない ---
    static inline float Clamp01(float v) noexcept {
        return (v < 0.0f) ? 0.0f : (v > 1.0f ? 1.0f : v);
    }
    float EaseSmoothStep01(float t) const noexcept {
        t = Clamp01(t);
        return t * t * (3.0f - 2.0f * t);
    }

    struct OrbitZoneXZ {
        float minX, maxX, minZ, maxZ;
    };
    std::vector<OrbitZoneXZ> m_orbitZones;  // 許可ゾーンのリスト

private:
    float m_duration = 0.30f;
    float m_stepAngleRad = nsK2EngineLow::Math::DegToRad(-45.0f);

    bool  m_isRotating = false;
    int   m_rotateQueue = 0;

    float m_elapsed = 0.0f;
    float m_targetAngleRad = 0.0f;
    float m_currentAngleRad = 0.0f;

    Player* m_player = nullptr;				//プレイヤー用のインスタンス。
    Vector3 m_cameraTarget;					//カメラの注視点。
    Vector3 m_leftScreenEdge;				//左画面端。
    Vector3 m_toCameraPos;					//注視点から視点に向かうベクトル。
    double m_toCameraPosmultiplier = 1.0f;	//カメラの倍率。
    bool m_stageTransitionFlag = false;		//ステージ遷移フラグ

    nsK2EngineLow::Vector3 m_playerPos{ 0,0,0 };
    nsK2EngineLow::Vector3 m_initialOffset{ 0,0,-200 };
    nsK2EngineLow::Vector3 m_followOffset{ 0,0,-200 };

    // --- ここを修正：Vector3 に nsK2EngineLow:: を付ける ---
    std::function<nsK2EngineLow::Vector3(void)> m_getPlayerPos;
};
