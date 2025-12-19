/*
 * @file ICameraBase.h
 * @brief カメラ基底クラス
 * @dote 2025/10/27 … クラス作成日。
 */
#pragma once
namespace nsK2EngineLow {
	class Camera;
}

class ICameraStrategy
{
public:
	ICameraStrategy() = default;
	virtual ~ICameraStrategy() = default;
	virtual bool Start() = 0;
    virtual void Update(nsK2EngineLow::Camera* pCamera, const float deltaTime) = 0;


public:
    inline void SetTargetRotationY(float angleDeg) { targetRotation_.SetRotationY(Math::DegToRad(angleDeg)); };


public:
    // ターゲットのY回転角度を取得。
    inline float GetTargetRotationY() const
    {
        Vector3 angles;
        float angleRad = 2.0f * atan2f(targetRotation_.y, targetRotation_.w);
        return Math::RadToDeg(angleRad);
    }


protected:
    Quaternion targetRotation_ = Quaternion::Identity;
    Quaternion currentRotation_ = Quaternion::Identity;
};
