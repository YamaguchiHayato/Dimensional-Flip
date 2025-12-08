#pragma once
#include <DirectXMath.h>

#include "Src/Camera/ICameraStrategy.h" // 親クラスのインクルード必須

class Player;

namespace app
{
    namespace camera
    {
        class StageEXCameraStrategy : public ::ICameraStrategy
        {
        public:
            StageEXCameraStrategy(::Player* pPlayer);
            virtual ~StageEXCameraStrategy() = default;

            bool Start() override { return true; };
            void Update(nsK2EngineLow::Camera* camera, const float deltaTime) override;

            inline void SetTargetRotationY(float angleDeg) { targetRotation_.SetRotationY(Math::DegToRad(angleDeg)); };

        private:
            ::Player* pPlayer_ = nullptr;

            Quaternion currentRotation_ = Quaternion::Identity;
            Quaternion targetRotation_ = Quaternion::Identity;
            float rotationSpeed_ = 4.0f;
        };
    } 
} 
