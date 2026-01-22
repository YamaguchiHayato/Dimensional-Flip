#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Stage/Gimmick/FireBall.h"


namespace app
{
    namespace enemyState
    {
        class BossAttackFireBallState : public IBossStrategy
        {
        public:
            BossAttackFireBallState() = default;
            virtual ~BossAttackFireBallState() = default;


        public:
            void Enter(app::enemy::Boss* pBoss)override;
            void Update() override;
            void Exit() override;
            bool IsFinished() const override;


        // 内部参照。
        private:
            // 火の玉を発射する処理。
            void ShotFireBall();


        // ヘルパー。
        private:
            // 発射中もPlayerの方を向かせる処理。
            void LookAtPlayerDirection();

            // 火の玉の射出角度を計算する処理。
            void CalculateFireBallDirection();
        private:
            app::enemy::Boss* pBoss_ = nullptr;
            app::gimmick::FireBall* pFireBall_ = nullptr;


        private:
            float timer_ = 0;
            float shootTimer_ = 0;


        // 火の玉の発射角度用変数群。
        private:
            Vector3 spawnPos_ = Vector3::Zero; // 発射位置。
            Vector3 targetDir = Vector3::Right;

            Quaternion rotation_ = Quaternion::Identity;
            Quaternion offSet_ = Quaternion::Identity;

            float currentAngle_ = 0.0f; // 現在の角度。
            float randomAngle_ = 0.0f;  // ばらつかせる角度。
            float angleOffset_ = 0.0f;  // 角度オフセット。
            float finalAngle_ = 0.0f;   // 最終的な角度。

        };
    }
}

