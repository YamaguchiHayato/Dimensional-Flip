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
    bool Start() override { return true; };
    void Update() override;

private:
    void InitCameraAspect()
    {
        float aspect = 16.0 / 9.0f;
        float zoomHeight = 25.0f;

        g_camera3D->SetHeight(zoomHeight);
        g_camera3D->SetWidth(zoomHeight * aspect);
    }
};
