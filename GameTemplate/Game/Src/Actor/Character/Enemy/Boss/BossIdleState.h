#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"


namespace app
{
    namespace enemyState
    {
        class BossIdleState : public IEnemyState
        {
        public:
            BossIdleState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossIdleState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            app::enemy::Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f; // 待機時間を計測。
        };
    }
}
