#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackSpearState : public IBossStrategy
        {
        public:
            BossAttackSpearState() = default;
            virtual ~BossAttackSpearState() = default;


        public:
            // IBossStrategyの仮想関数のオーバーライド。
            void Enter(app::enemy::Boss* pBoss) override;
            void Update() override;
            void Exit() override;


        // ゲッター。
        public:
            // 攻撃アニメーションが終了しているか。
            inline bool IsFinished() const override
            {
                return timer_ >= 1.5f;
            }


        // セッター。
        public:
            // 開始地点をセット。
            inline void SetStartPos(const Vector3& pos)
            {
                startPos_ = pos;
            }

        private:
            // 槍攻撃の生成。
            void CreateSpearAttack();


        private:
            app::enemy::Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f;
            float centerX_ = 0.0f;
            float randomOffset_ = 0.0f;
            float startX_ = 0.0f;
            float endX_ = 0.0f;

            int pattern_ = 0;

            bool isAttackSpawned_ = false;

            Vector3 startPos_ = Vector3::Zero;
            Vector3 posBackStart_ = Vector3::Zero;
            Vector3 posBackTarget_ = Vector3::Zero;
            Vector3 posFrontStart_ = Vector3::Zero;
            Vector3 posFrontTarget_ = Vector3::Zero;
        };

    }
}
