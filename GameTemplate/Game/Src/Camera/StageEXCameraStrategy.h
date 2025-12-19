#pragma once
#include "Src/Camera/ICameraStrategy.h" // 親クラスのインクルード必須


class Player;


namespace app
{
    namespace camera
    {
        class StageEXCameraStrategy : public ICameraStrategy
        {
        public:
            StageEXCameraStrategy(Player* pPlayer);
            virtual ~StageEXCameraStrategy() = default;


        public:
            bool Start() override { return true; };
            void Update() override;


        private:
            Quaternion currentRotation_ = Quaternion::Identity;
        };
    } 
} 
