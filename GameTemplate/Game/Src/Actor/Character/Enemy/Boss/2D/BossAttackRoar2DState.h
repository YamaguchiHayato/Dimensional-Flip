#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app{
    namespace enemy{
        class Boss;
    }

    namespace gimmick{
        class FireBall;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackRoar2DState : public IBossStrategy
        {
        public:
            BossAttackRoar2DState() = default;
            virtual ~BossAttackRoar2DState() = default;


        public:
            void Enter(app::enemy::Boss* pBoss) override;
            void Update() override;
            void Exit() override;
            bool IsFinished() const override;


        private:
            // Playerの方に向かせる処理。
            void LookAtPlayerDirection();

        private:
            app::enemy::Boss* pBoss_ = nullptr;
            app::gimmick::FireBall* pFireBall_ = nullptr;

        private:
            float timer_ = 0.0f;

            Quaternion rot_ = Quaternion::Identity;
            Quaternion offSetRot_ = Quaternion::Identity;
        };


    }
}

