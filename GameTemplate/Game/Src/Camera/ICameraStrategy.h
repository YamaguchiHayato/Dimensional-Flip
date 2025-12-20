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


    inline float LerpFloat(float start, float end, float speed)
    {
        float t = speed * g_gameTime->GetFrameDeltaTime();
        if (t > 1.0f)
            t = 1.0f;
        return start + (end - start) * t;
    }

    // playerY   : プレイヤーの現在のY座標
    // threshold : 追従を開始する高さライン（例: 100.0f）
    // offset    : カメラが維持したい高さオフセット（Sideなら60固定、FollowならStick入力等）
    inline float CalculateThresholdY(float playerY, float threshold, float offset)
    {
        float destY = 0.0f;

        // ラインより低い場合：地面基準の高さ（固定）
        if (playerY < threshold)
        {
            destY = offset;
        }
        // ラインを超えた場合：超えた分だけ持ち上げる
        else
        {
            float excess = playerY - threshold;
            destY = offset + excess;
        }
        return destY;
    }


protected:
    Player* pPlayer_ = nullptr;


protected:
    Quaternion targetRotation_ = Quaternion::Identity;
};
