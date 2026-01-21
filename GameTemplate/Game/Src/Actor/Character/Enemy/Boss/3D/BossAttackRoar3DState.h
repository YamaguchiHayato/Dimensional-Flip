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
        class BossAttackRoar3DState : public IBossStrategy
        {
        public:
            BossAttackRoar3DState() = default;
            virtual ~BossAttackRoar3DState() = default;


        public:
            // 継承。
            void Enter(app::enemy::Boss* pBoss) override;

            void Update() override;

            void Exit() override;

            bool IsFinished() const override;

        private:
            app::enemy::Boss* pBoss_ = nullptr;


        private:    
            float timer_ = 0.0f;
            bool isAttackSpawned_ = false;
        };
    } 
} 
