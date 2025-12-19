/*
 * @file ICameraBase.h
 * @brief カメラ基底クラス
 * @dote 2025/10/27 … クラス作成日。
 */

class Player;

#pragma once
namespace nsK2EngineLow
{
    class Camera;
}

class ICameraStrategy
{
public:
    // todo for test Player* pPlayerを基底クラスに追加出来るのか試す
    ICameraStrategy() = default;
    virtual ~ICameraStrategy() = default;


public:
    virtual bool Start() = 0;
    virtual void Update() = 0;


public:
    inline void SetTargetRotationY(float angleDeg) { targetRotation_.SetRotationY(Math::DegToRad(angleDeg)); };


protected:
    static Vector3 Lerp(float t, const Vector3& v0, const Vector3& v1)
    {
        DirectX::XMVECTOR resultVec = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&v0.vec), DirectX::XMLoadFloat3(&v1.vec), t);

        Vector3 result;
        DirectX::XMStoreFloat3(&result.vec, resultVec);
        return result;
    }


protected:
    Player* pPlayer_ = nullptr;


protected:
    Quaternion targetRotation_ = Quaternion::Identity;
};
