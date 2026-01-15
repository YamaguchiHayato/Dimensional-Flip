#pragma once
#include "Src/Actor/Character/Enemy/Fall/FallEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemy
    {
        class FallEnemyRiseState : public IEnemyState
        {
        public:
            FallEnemyRiseState(FallEnemy* pFallEnemy) : pFallEnemy_(pFallEnemy) {}
            virtual ~FallEnemyRiseState() = default;

            void Enter() override;
            void Update() override;
            void Exit() override; // 座標補正のため必要かも
            bool RequestID(uint8_t& request) override;

        private:
            FallEnemy* pFallEnemy_ = nullptr;


        private:
            bool isArrived_ = false; 
        };
    }
}

