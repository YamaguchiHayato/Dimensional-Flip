#include "stdafx.h"

#include "BossAttackFireBallState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto SHOOT_INTERVAL = 0.5f;  // 連射速度をアップ（弾幕感）
    const auto BARRAGE_DURATION = 6.0f; // 弾幕を飛ばす時間
    const auto FIREBALL_SPEED = 60.0f; // 速度
    const float EFFECT_TILT_ANGLE = 90.0f;
} // namespace

namespace app
{
    namespace enemyState
    {
        void BossAttackFireBallState::Enter(app::enemy::Boss* pBoss)
        {
            // 初期値をセットする。
            SetInitialValue(pBoss);

            // 初期ステップをセット。
            SetCurrentStep(AttackStep::Breath);

            // アニメーションをセットする。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, true, 0.1f);

            // 移動ベクトルをセット。
            pBoss_->SetMoveSpeed(Vector3::Zero);
        }


        void BossAttackFireBallState::Update()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;
            stepTimer_ += deltaTime; 

            // 発射中もPlayerの方を向く（モデルの向き）
            LookAtPlayerDirection();

            // ステップ（予兆 or 弾幕）の更新処理。
            UpdateBreastStep(deltaTime);
        }


        void BossAttackFireBallState::Exit()
        {
            if (pBreathEffect_)
            {
                DeleteGO(pBreathEffect_);
                pBreathEffect_ = nullptr;
            }

            // 次の攻撃までのインターバル。
            pBoss_->SettNextInterval(2.5f);
        }


        bool BossAttackFireBallState::IsFinished() const
        {
            return currentStep_ == AttackStep::Finish;
        }


        void BossAttackFireBallState::ShotFireBall()
        {
            // 0:左, 1:右, 2:上, 3:下 からランダムに選択
            int side = rand() % 4;
            float offsetX = 80.0f; // 画面端
            float offsetY = 80.0f;

            switch (side)
            {
            case 0: // 左 -> 右へ
                spawnPos_.Set(-offsetX, static_cast<float>(rand() % 60 + 10), 0.0f);
                moveDirection_ = Vector3::Right;
                break;

            case 1: // 右 -> 左へ
                spawnPos_.Set(offsetX, static_cast<float>(rand() % 60 + 10), 0.0f);
                moveDirection_ = Vector3::Left;
                break;

            case 2: // 上 -> 下へ
                spawnPos_.Set(static_cast<float>(rand() % 120 - 60), offsetY, 0.0f);
                moveDirection_ = Vector3::Down;
                break;

            case 3: // 下 -> 上へ
                spawnPos_.Set(static_cast<float>(rand() % 120 - 60), -20.0f, 0.0f);
                moveDirection_ = Vector3::Up;
                break;
            }

            auto* fireBall = NewGO<app::gimmick::FireBall>(0);

            // 以前の要望通りサイズを大きく
            fireBall->SetScale(Vector3::One * 1.25);

            // 進行方向に対してエフェクトを倒す設定
            Quaternion tilt;
            tilt.SetRotationDegX(EFFECT_TILT_ANGLE);
            fireBall->SetRotationOffset(tilt);

            // 方向指定で射出
            fireBall->SetDirection(spawnPos_, moveDirection_, FIREBALL_SPEED);
        }


        void BossAttackFireBallState::UpdateBreastStep(float dTime)
        {
            switch (currentStep_)
            {
            case AttackStep::Breath:
                UpdateBreath();
                break;

            case AttackStep::Barrage:
                UpdateBarrage(dTime);
                break;

            default:
                break;
            }
        }


        void BossAttackFireBallState::UpdateBreath()
        {
            if (!pBreathEffect_)
            {
                pBreathEffect_ = NewGO<app::gimmick::FireBall>(0);
                pBreathEffect_->SetScale(Vector3(5.0f, 5.0f, 5.0f));
                pBreathEffect_->SetSpeed(0.0f);
                pBreathEffect_->SetCollisionEnable(false); // 当たり判定で消えないようにする


                //　垂直方向の予備動作エフェクトをボスの口元の角度にセット。
                Quaternion tilt = Quaternion::Identity;
                tilt.SetRotationDegY(EFFECT_TILT_ANGLE);
                pBreathEffect_->SetRotationOffset(tilt);
            }

            if (pBreathEffect_)
            {
                pBreathEffect_->SetPosition(pBoss_->GetWeakPoint());
                pBreathEffect_->SetRotation(pBoss_->GetRot());
            }

            // 1秒間の予兆の後に弾幕へ移行
            if (stepTimer_ >= 1.0f)
            {
                if (pBreathEffect_)
                {
                    DeleteGO(pBreathEffect_);
                    pBreathEffect_ = nullptr;
                }

                currentStep_ = AttackStep::Barrage;
                stepTimer_ = 0.0f;
                shootTimer_ = 0.0f;

                pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, true, 0.1f);
            }
        }


        void BossAttackFireBallState::UpdateBarrage(float dTime)
        {
            shootTimer_ += dTime;
            if (shootTimer_ >= SHOOT_INTERVAL)
            {
                shootTimer_ = 0.0f;
                ShotFireBall();
            }

            // 指定時間経過したら終了へ
            if (stepTimer_ >= BARRAGE_DURATION)
            {
                currentStep_ = AttackStep::Finish;
            }
        }


        void BossAttackFireBallState::LookAtPlayerDirection()
        {
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                Vector3 diff = pPlayer->GetPlayerPos() - pBoss_->GetPos();
                diff.y = 0;
                diff.z = 0;
                if (diff.LengthSq() > 0.1f)
                {
                    rotation_.SetRotationYFromDirectionXZ(diff);
                    offSet_.SetRotationDegY(-90.0f);
                    pBoss_->SetRot(rotation_ * offSet_);
                }
            }
        }
    } 
} 
