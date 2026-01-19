#pragma once
#include "Src/Actor/Character/Enemy/Fall/FallEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemy
    {
        class FallEnemyFallState : public IEnemyState
        {
        public:
            FallEnemyFallState(FallEnemy* pFallEnemy) : pFallEnemy_(pFallEnemy) {}
            virtual ~FallEnemyFallState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override {} 
            bool RequestID(uint8_t& request) override;


        private:
            FallEnemy* pFallEnemy_ = nullptr;


        private:    
            bool isGround_ = false; // 地面に着いたか
        };

    } 
} 
