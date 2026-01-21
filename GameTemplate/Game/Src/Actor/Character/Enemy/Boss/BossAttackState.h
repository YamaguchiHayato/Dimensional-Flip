#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"


namespace app
{
    namespace enemyState
    {
        class BossAttackState : public IEnemyState
        {
        public:
            BossAttackState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossAttackState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            // どの攻撃を実行するか決める関数。
            // 3D視点版。
            void DecideStrategy3D();

            // 2D視点版。
            void DecideStrategy2D();

        private:
            app::enemy::Boss* pBoss_ = nullptr;

            std::unique_ptr<app::enemyState::IBossStrategy> currentState_ = nullptr; // 現在の攻撃ステート。 
        };

    }
}
