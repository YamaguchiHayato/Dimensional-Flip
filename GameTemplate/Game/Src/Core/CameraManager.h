#pragma once
#include <memory>

#include "Src/Camera/ICameraStrategy.h"

// 前方宣言
class Player;
class Boss;

// カメラモード列挙型
enum class CameraMode : uint8_t
{
    mode2_5D,    // 横スクロール
    mode3D,      // 3D自由視点
    modeBoss,    // ボス戦演出
    modeStageEX, 
};

class CameraManager : public IGameObject
{
public:
    CameraManager() = default;
    virtual ~CameraManager() = default;

    bool Start() override;
    void Update() override;

    // カメラモード。
    void Request2DMode();
    void Request3DMode();
    void RequestBossMode(float targetAngleDegrees = 0.0f);
    void RequestStageExMode();

    // 回転リクエスト
    void Request3DModeRot(float targetAngleDegrees);

    // ゲッター
    CameraMode GetCurrentCameraMode() const { return currentMode_; }
    ICameraStrategy* GetCurrentStrategy() { return pCameraStrategy_.get(); }

private:
    std::unique_ptr<ICameraStrategy> pCameraStrategy_;
    CameraMode currentMode_ = CameraMode::mode2_5D;

    Player* pPlayer_ = nullptr;
    Boss* pBoss_ = nullptr;
};
