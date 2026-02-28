#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }

    namespace gimmick{
        class RoarWave;
    }
} 

namespace app
{
    namespace enemyState
    {
        class BossAttackRoar3DState : public IBossStrategy
        {
        public:
            BossAttackRoar3DState() = default;
            virtual ~BossAttackRoar3DState() = default;


        public:
            // 継承。
            // 攻撃開始時の初期化。
            void Enter(app::enemy::Boss* pBoss) override;
            // 攻撃中の更新処理。
            void Update() override;
            // 終了判定。
            void Exit() override;
            // 攻撃が終わったかを判定する。
            bool IsFinished() const override;


        private:
            // 攻撃生成関数。
            void CreateRoarWave();

            // 攻撃開始時にPlayerの方向を向く。
            void UpdateLookAtPlayer();


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            app::gimmick::RoarWave* pRoarWave_ = nullptr;


        private:    
            float timer_ = 0.0f;
            float angleOffset_ = 0.0f;

            bool isAttackSpawned_ = false;

            Vector3 bossPosition_ = Vector3::Zero;
            Vector3 forwardDirection_ = Vector3::Front;
            Vector3 moveDirection_ = Vector3::Zero;
            Vector3 spawnPosition_ = Vector3::Zero;
            Vector3 toPlayer_ = Vector3::Zero;
            Vector3 currentScale_ = Vector3::One;

            Quaternion bossAngle_ = Quaternion::Identity;
            Quaternion sqredAngle_ = Quaternion::Identity;
            Quaternion lookAtPlayerAngle_ = Quaternion::Identity;
        };
    } 
} 
