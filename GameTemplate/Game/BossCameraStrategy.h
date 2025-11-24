#pragma once

// Boss戦用カメラ戦略クラス。
#include "Src/Camera/ICameraStrategy.h"

// 戦闘中のカメラモード。
enum class BattleViewMode : uint8_t
{
    SidwView = 0, // 側面視点 : 攻撃・移動用。
    DepthView,    // 奥行き視点 : 回避用。
};

// ボス戦カメラの状態。
enum class BossCameraState : uint8_t
{
    Appearance = 0, // 登場中。（イベントカメラ）
    Battle,         // 戦闘中。（プレイヤー操作可能）
};

class Boss;
class BossCameraStrategy : public ICameraStrategy
{
public:
    BossCameraStrategy() = default;
    virtual ~BossCameraStrategy() = default;

    bool Start() override;
    void Update(nsK2EngineLow::Camera* pCamera, const float deltaTime) override;

    void BossAppearanceCamera();
    void BattleCamera(nsK2EngineLow::Camera* pCamera, const float deltaTime);
    void BossCameraGetState(nsK2EngineLow::Camera* pCamera, const float deltaTime);
    // 戦闘中のカメラの視点切替要求。
    inline void RequestBattleView(BattleViewMode mode)
    {
        if (currenState_ == BossCameraState::Battle)
            currentViewMode_ = mode;
    }

private:
    Boss* pBoss_ = nullptr;

private:
    BossCameraState currenState_ = BossCameraState::Appearance; // 初期状態は登場中
    BattleViewMode currentViewMode_ = BattleViewMode::SidwView; // 初期視点は側面

    float eventTimerLapse_ = 0.0f; // イベントカメラの経過時間
};
