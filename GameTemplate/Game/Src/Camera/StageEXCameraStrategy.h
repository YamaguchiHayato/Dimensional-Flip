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

            // 俯瞰視点用のオフセット設定
            Vector3 baseOffset_{0.0f, 800.0f, -400.0f};

            Quaternion currentRotation_ = Quaternion::Identity;
            Quaternion targetRotation_ = Quaternion::Identity;
            float rotationSpeed_ = 4.0f;
        };
    } 
} 
