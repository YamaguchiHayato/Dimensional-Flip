#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackSpearState : public IBossStrategy
        {
        public:
            BossAttackSpearState() = default;
            virtual ~BossAttackSpearState() = default;


        public:
            // IBossStrategyの仮想関数のオーバーライド。
            void Enter(app::enemy::Boss* pBoss) override;

            void Update() override;

            void Exit() override;

            inline bool IsFinished() const override
            {
                return !pBoss_->IsPlayingAnimation();
            }


        private:
            void CreateSpearAttack();

        private:
            app::enemy::Boss* pBoss_ = nullptr;
            float timer_ = 0.0f;
            bool isAttackSpawned_ = false;
        };

    }
}
