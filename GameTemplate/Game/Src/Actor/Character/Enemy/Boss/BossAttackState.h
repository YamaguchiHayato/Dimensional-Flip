#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"


namespace app
{
    namespace enemy
    {
        class BossAttackState : public IEnemyState
        {
        public:
            BossAttackState(Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossAttackState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f; // 攻撃時間を計測。

            bool isAttackSpawned_ = false; // 攻撃オブジェクトが生成済みかチェック。

            AttackType currentAttackType_;
        };

    }
}
