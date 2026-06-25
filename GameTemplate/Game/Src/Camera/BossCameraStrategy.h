#pragma once
#include "Src/Camera/ICameraStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

class CameraManager;

namespace app
{
    namespace camera
    {
        class BossCameraStrategy : public ICameraStrategy
        {
        public:
            BossCameraStrategy(Player* pPlayer) : pPlayer_(pPlayer) {};
            virtual ~BossCameraStrategy() = default;


        public:
            bool Start() override;
            void Update() override;


        public:
            // クライミングカメラのターゲットY座標を設定。
            void SetClimbingTargetY(float targetY)
            {
                climbingTargetY_ = targetY;
            }


        private:
            // 足場を登り用のカメラワーク。
            void UpdateClimbingCamera();


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            Player* pPlayer_ = nullptr;


        private:
            float climbingTargetY_ = 0.0f;
        };
    }
}

