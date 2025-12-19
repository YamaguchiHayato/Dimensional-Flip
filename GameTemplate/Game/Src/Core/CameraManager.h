#pragma once
#include <memory>

#include "Src/Camera/ICameraStrategy.h"

// 前方宣言
class Player;
class Boss;

// カメラモード列挙型
enum class CameraMode : uint8_t
{
    mode2D,    // 横スクロール
    mode3D,      // 3D自由視点
    modeBoss,    // ボス戦演出
    modeStageEX, 
};


class CameraManager : public IGameObject
{
public:
    CameraManager() = default;
    virtual ~CameraManager() = default;


public:
    bool Start() override;
    void Update() override;


public:
    // カメラモード。
    void Request2DMode();
    void Request3DMode();
    void RequestBossMode(float targetAngleDegrees = 0.0f);
    void RequestStageExMode();


private:
    template <typename CameraType>
    void RequestCameraMode(const float angle, CameraMode cameraMode, const nsK2EngineLow::Camera::EnUpdateProjMatrixFunc mode);

    void ChangeCamera();

// ゲッター
public:
    CameraMode GetCurrentCameraMode() const { return currentMode_; }

private:
    Player* pPlayer_ = nullptr;
    std::unique_ptr<ICameraStrategy> pCameraStrategy_;
    CameraMode currentMode_ = CameraMode::mode2D;
};

