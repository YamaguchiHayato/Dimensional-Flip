#pragma once
#include "ICameraStrategy.h"
#include "math/Math.h"

enum class OrbitMode
{
    XZ, /* X軸 → Z軸への回転用。*/
    YZ  /* Y軸 → Z軸への回転用。*/
};

class GameCamera;
class OrbitCameraStrategy : public ICameraStrategy
{
public:
    OrbitCameraStrategy(float step = -45.0f, float speed = 0.3f, OrbitMode mode = OrbitMode::XZ) :
        m_step(step),
        m_speed(speed),
        m_mode(mode),
        // ★メンバー初期化子リストで初期化を行うように変更
        m_stepAngleRad(Math::DegToRad(step)),
        m_duration(std::fabs(Math::DegToRad(step) / speed))
    {
    }
    void Update(GameCamera* owner) override;
    void SetMode(OrbitMode mode) { m_mode = mode; }

private:
    void ApplyOrbit(Vector3& offset, float angleNow);
    void BeginOrbitStep(GameCamera* owner);
    void TickOrbit(GameCamera* owner, float dt);

    float m_stepAngleRad;
    float m_duration;

    bool  m_isRotating = false;
    int   m_rotateQueue = 0;
    float m_elapsed = 0.0f;
    float m_targetAngleRad = 0.0f;
    float m_currentAngleRad = 0.0f;
    nsK2EngineLow::Vector3 m_initialOffset;

private:
    float m_step;
    float m_speed;
    OrbitMode m_mode;
};

