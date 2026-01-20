#pragma once
#include "Src/camera/ICameraStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

class Player;
class SideCameraStrategy : public ICameraStrategy
{
public:
    // 暗黙的な型変換を禁止する。
    explicit SideCameraStrategy(Player* pPlayer);
    virtual ~SideCameraStrategy() {};

public:
    bool Start() override;
    void Update() override;

private:
    void InitCameraAspect(float zoomHeight)
    {
        float aspect = 16.0 / 9.0f;

        g_camera3D->SetHeight(zoomHeight);
        g_camera3D->SetWidth(zoomHeight * aspect);
    }

    // ステージ用カメラ。
    void StageCamera();

    // ボス戦用カメラ。
    void BossCamera();

private:
    app::enemy::Boss* pSecondTarget_ = nullptr;
};
