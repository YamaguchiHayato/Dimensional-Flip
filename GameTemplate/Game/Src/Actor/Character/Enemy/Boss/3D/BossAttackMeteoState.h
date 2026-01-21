#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackMeteoState : public IBossStrategy
        {
        public:
            BossAttackMeteoState() = default;
            virtual ~BossAttackMeteoState() = default;


        public:
            // IBossStrategyの仮想関数のオーバーライド。
            void Enter(app::enemy::Boss* pBoss)override;
            void Update() override;
            void Exit() override;
            bool IsFinished() const override;

        private:
            void CreateMeteoAttack();


        private:
            app::enemy::Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f;
            bool isAttackSpawned_ = false;
        };

    }
}

