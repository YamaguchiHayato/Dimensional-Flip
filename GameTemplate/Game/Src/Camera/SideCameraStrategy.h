#pragma once
#include "Src/camera/ICameraStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

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
    // カメラのアスペクト比を初期化する。
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

    // 座標を制限範囲内に納める。
    void ApplyScreenRock(Vector3& cameraPos);

// セッター。
public:
    // カメラの制限範囲を設定する。
    inline void SetCameraLimit(const Vector3& rangeMin, const Vector3 rangeMax)
    {
        rangeMin_ = rangeMin;
        rangeMax_ = rangeMax;
        isScreenRock_ = true;
    }


private:
    app::enemy::Boss* pSecondTarget_ = nullptr;


// スクリーンロック用変数群。
private:
    bool isScreenRock_ = false;
    Vector3 rangeMin_ = Vector3::Zero;
    Vector3 rangeMax_ = Vector3::Zero;
};
