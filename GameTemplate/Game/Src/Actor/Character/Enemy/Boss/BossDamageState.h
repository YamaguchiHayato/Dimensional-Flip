#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"


namespace app
{
    namespace enemyState
    {
        class BossDamageState : public IEnemyState
        {
        public:
            BossDamageState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossDamageState() = default;


        public:
            void Enter() override;
            void Update() override {};
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            app::enemy::Boss* pBoss_ = nullptr;

        };

    }
}
