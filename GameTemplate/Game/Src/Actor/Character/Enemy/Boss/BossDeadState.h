#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossDeadState : public IEnemyState
        {
        public:
            BossDeadState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossDeadState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            app::enemy::Boss* pBoss_ = nullptr;
        };

    }
}

