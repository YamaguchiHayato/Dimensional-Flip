#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

namespace app
{
    namespace enemyState
    {
        class BossAttackMeteo2DState : public IBossStrategy
        {
        public:
            BossAttackMeteo2DState() = default;
            virtual ~BossAttackMeteo2DState() = default;


        public:
            void Enter(app::enemy::Boss* pBoss) override;
            void Update()override;
            void Exit()override;


        // ゲッター。
        public:
            // 攻撃終了判定。
            inline bool IsFinished() const override
            {
                return isFinished_;
            }


        private:
            void SpawnMeteoLine();


        private:
            app::enemy::Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f;
            bool isAttackSpawned_ = false;
            bool isFinished_ = false;
        };

    }
}

