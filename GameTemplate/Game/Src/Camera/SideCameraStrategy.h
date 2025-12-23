#pragma once
#include "Src/camera/ICameraStrategy.h"

class Player;
class SideCameraStrategy : public ICameraStrategy
{
public:
    // 暗黙的な型変換を禁止する。
	explicit SideCameraStrategy(Player* pPlayer);
    virtual ~SideCameraStrategy() {};


public:
	bool Start()override { return true; };
	void Update() override;


public:
    inline float CameraLerp(float start, float end, float speed)
    {
        float t = speed * g_gameTime->GetFrameDeltaTime();

        // tが1.0を超えないようにする。
        if (t > 1.0f)
            t = 1.0f;

        return start + (end - start) * t;
    }

private:
    inline void SetCameraAspect()
    {
        auto aspect = 16.0 / 9.0f;
        auto zoomHeight = 15.0f;

        g_camera3D->SetHeight(zoomHeight);
        g_camera3D->SetWidth(zoomHeight * aspect);

    }

private:
    Vector3 currentPos_ = Vector3::Zero;
	Quaternion currentRotation_ = Quaternion::Identity;
};
