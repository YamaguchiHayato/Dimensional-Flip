#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/FireBall.h"

#include "Src/Actor/Character/Player/Player.h"

namespace app
{
    namespace enemyState
    {
        enum class AttackStep : uint8_t
        {
            Breath,  // 口元から火を吹く（溜め/予兆）
            Barrage, // 画面奥から火の玉が降ってくる
            Finish   // 攻撃終了。
        };


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


        // セッター。
        public:
            // 進行方向を決める。
            inline void SetFireBallDirection(const Vector3& direction)
            {
                moveDirection_ = direction;
            }

            // 現在の状態をセット。
            inline void SetCurrentStep(AttackStep step)
            {
                currentStep_ = step;
                stepTimer_ = 0.0f;
            }

            // ステートの初期値をセット。
            inline void SetInitialValue(app::enemy::Boss* pBoss)
            {
                pBoss_ = pBoss;
                timer_ = 0.0f;
                shootTimer_ = 0.0f;
            }

        // 内部参照。
        private:
            // 火の玉を発射する処理。
            void ShotFireBall();

            // 各状態の更新処理。
            // Breath状態。
            void UpdateBreath();

            // Barrage状態。
            void UpdateBarrage(float dTime);


        // ヘルパー。
        private:
            // 発射中もPlayerの方を向かせる処理。
            void LookAtPlayerDirection();

            // 火の玉の射出角度を計算する処理。
            void CalculateFireBallDirection();


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            app::gimmick::FireBall* pFireBall_ = nullptr;

            Player* pPlayer = nullptr;

        private:
            float timer_ = 0;
            float shootTimer_ = 0;


        // 火の玉の発射角度用変数群。
        private:
            Vector3 spawnPos_ = Vector3::Zero; // 発射位置。
            Vector3 targetDir = Vector3::Right;
            Vector3 moveDirection_ = Vector3::Zero; // 移動方向。

            Quaternion rotation_ = Quaternion::Identity;
            Quaternion offSet_ = Quaternion::Identity;

            float currentAngle_ = 0.0f; // 現在の角度。
            float randomAngle_ = 0.0f;  // ばらつかせる角度。
            float angleOffset_ = 0.0f;  // 角度オフセット。
            float finalAngle_ = 0.0f;   // 最終的な角度。


        // 挙動用変数群。
        private:
            AttackStep currentStep_ = AttackStep::Breath;
            float stepTimer_ = 0.0f;
            float moveSpeed_ = 0.0f;

            // 口元に追従させるブレス用のエフェクト。
            app::gimmick::FireBall* pBreathEffect_ = nullptr;


        private:
            void UpdateBreastStep(float dTime);
        };
    }
}

