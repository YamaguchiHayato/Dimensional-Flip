#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"


namespace app
{
    namespace enemy
    {
        class BossDamageState : public IEnemyState
        {
        public:
            BossDamageState(Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossDamageState() = default;


        public:
            void Enter() override;
            void Update() override {};
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            Boss* pBoss_ = nullptr;

        };

    }
}
