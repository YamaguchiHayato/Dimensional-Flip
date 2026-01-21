#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemy.h"

namespace app
{
    namespace enemyState
    {
        class FallEnemyLandState : public IEnemyState
        {
        public:
            FallEnemyLandState(app::enemy::FallEnemy* pFallEnemy) : pFallEnemy_(pFallEnemy) {};
            virtual ~FallEnemyLandState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            app::enemy::FallEnemy* pFallEnemy_ = nullptr;


        private:
            float timer_ = 0.0f;      // 計測タイマー。
            bool isTimeOver_ = false; // 時間切れフラグ。
        };

    }
}
