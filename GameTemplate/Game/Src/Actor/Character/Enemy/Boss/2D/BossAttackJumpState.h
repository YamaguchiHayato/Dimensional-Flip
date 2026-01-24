#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}


namespace app
{
    namespace enemyState
    {
        class BossAttackJumpState : public IBossStrategy
        {
        public:
            BossAttackJumpState() =default;
            virtual ~BossAttackJumpState() =default;


        private:
            void Enter(app::enemy::Boss* pBoss)override;
            void Update() override;
            void Exit() override;
            bool IsFinished() const override;


        private:
            void UpdateState();
            void UpdatePrepare();
            void UpdateJumping();
            void UpdateLanding();


        private:
            inline void AddJumpCount()
            {
                jumopCount_++;
            }


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            app::enemyStatus::JumpStep step_ = app::enemyStatus::JumpStep::Prepare;


        private:
            Vector3 startPos_ = Vector3::Zero;
            Vector3 targetPos_ = Vector3::Zero;
            Vector3 velocity_ = Vector3::Zero;


            Quaternion faceAngle_ = Quaternion::Identity;


            float timer_ = 0.0f;
            int jumopCount_ = 3;
        };

    }
}

