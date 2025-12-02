#pragma once
#include "Src/Camera/ICameraStrategy.h"

class CameraManager;
class Boss;
class Player;

enum class BattleViewMode : uint8_t
{
    SidwView = 0, // 側面視点
    DepthView,    // 奥行き視点
};

// ボス戦カメラの状態
enum class BossCameraState : uint8_t
{
    Appearance = 0, // 登場中（イベントカメラ）
    Battle,         // 戦闘中（プレイヤー操作可能）
};

namespace app
{
    namespace camera
    {
        class BossCameraStrategy : public ICameraStrategy
        {
        public:
            BossCameraStrategy(Player* pPlayer);
            virtual ~BossCameraStrategy() = default;

            bool Start() override;
            void Update(nsK2EngineLow::Camera* pCamera, const float deltaTime) override;

        private:
            // 内部処理関数
            void BossAppearanceCamera(nsK2EngineLow::Camera* pCamera);
            void BattleCamera(nsK2EngineLow::Camera* pCamera, const float deltaTime);
            void BossCameraGetState(nsK2EngineLow::Camera* pCamera, const float deltaTime);

        public:
            // 戦闘中のカメラの視点切替要求（必要に応じて使用）
            inline void RequestBattleView(BattleViewMode mode)
            {
                if (currenState_ == BossCameraState::Battle)
                    currentViewMode_ = mode;
            }

        private:
            Boss* pBoss_ = nullptr;
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;

            BossCameraState currenState_ = BossCameraState::Appearance; // 初期状態
            BattleViewMode currentViewMode_ = BattleViewMode::SidwView; // 初期視点

            float eventTimerLapse_ = 0.0f; // 演出経過時間
        };
    }
}

