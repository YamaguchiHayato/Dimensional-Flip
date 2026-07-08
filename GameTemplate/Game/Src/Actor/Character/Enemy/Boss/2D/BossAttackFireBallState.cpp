#include "stdafx.h"

#include "BossAttackFireBallState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto SHOOT_INTERVAL = 0.5f;      //! 連射速度をアップ。（弾幕感）
    const auto BARRAGE_DURATION = 6.0f;   //! 弾幕を飛ばす時間。
    const auto FIREBALL_SPEED = 60.0f;    //! 速度。
    const float EFFECT_TILT_ANGLE = 90.0f;
} // namespace

namespace app
{
    namespace enemyState
    {
        void BossAttackFireBallState::Enter(app::enemy::Boss* pBoss)
        {
            /* 初期値をセット。*/
            SetInitialValue(pBoss);

            /* 初期ステップをセットする。。*/
            SetCurrentStep(AttackStep::Breath);

            /* アニメーションを再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, true, 0.1f);

            /* 移動速度を0にする。*/
            pBoss_->SetMoveSpeed(Vector3::Zero);

            /* 攻撃種類 UI へ fireBall アイコンを通知（委譲経路）。 */
            pBoss_->SetAttackType(app::enemyStatus::AttackType::FireBall);
        }


        void BossAttackFireBallState::Update()
        {
            /* 経過時間を取得。*/
            auto deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;
            stepTimer_ += deltaTime; 

            /* Playerの方向を見る。*/
            LookAtPlayerDirection();

            /* 火炎級の動きを更新する。*/
            UpdateBreastStep(deltaTime);
        }


        void BossAttackFireBallState::Exit()
        {
            /* 経過時間をリセット。*/
            if (pBreathEffect_)
            {
                /* エフェクトを削除。*/
                DeleteGO(pBreathEffect_);
                pBreathEffect_ = nullptr;
            }

            /* 次の攻撃までのインターバルを設定。*/
            pBoss_->SettNextInterval(2.5f);

            /* 攻撃種類 UI へ fireBall アイコンを通知（委譲経路）。 */
            pBoss_->SetAttackType(app::enemyStatus::AttackType::FireBall);

        }


        bool BossAttackFireBallState::IsFinished() const
        {
            /* 現在のステップがFinishかどうかを返す。*/
            return currentStep_ == AttackStep::Finish;
        }


        void BossAttackFireBallState::ShotFireBall()
        {
            /* 画面端からランダムに出現させる。*/
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

            /* FireBallを生成。*/
            auto* fireBall = NewGO<app::gimmick::FireBall>(0);

            /* 位置を設定。*/
            fireBall->SetScale(Vector3::One * 1.25);

            /* 回転軸を設定。*/
            Quaternion tilt;
            tilt.SetRotationDegX(EFFECT_TILT_ANGLE);
            fireBall->SetRotationOffset(tilt);
            fireBall->SetDirection(spawnPos_, moveDirection_, FIREBALL_SPEED);
        }


        void BossAttackFireBallState::UpdateBreastStep(float dTime)
        {
            /* 現在のステップに応じて更新処理を行う。*/ 
            switch (currentStep_)
            {
            /* ブレスの予備動作。*/
            case AttackStep::Breath:
                UpdateBreath();
                break;

            /* 弾幕攻撃。*/
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
                /* ブレスの予備動作エフェクトを生成。*/
                pBreathEffect_ = NewGO<app::gimmick::FireBall>(0);
                pBreathEffect_->SetScale(Vector3(5.0f, 5.0f, 5.0f));
                pBreathEffect_->SetSpeed(0.0f);
                pBreathEffect_->SetCollisionEnable(false); // 当たり判定で消えないようにする

                /* エフェクトの回転を設定。*/
                Quaternion tilt = Quaternion::Identity;
                tilt.SetRotationDegY(EFFECT_TILT_ANGLE);
                pBreathEffect_->SetRotationOffset(tilt);
            }

            if (pBreathEffect_)
            {
                /* ブレスの予備動作エフェクトをボスの弱点位置に追従させる。*/
                pBreathEffect_->SetPosition(pBoss_->GetWeakPoint());
                pBreathEffect_->SetRotation(pBoss_->GetRot());
            }

            /* 1秒経過したら弾幕攻撃に移行する。*/
            if (stepTimer_ >= 1.0f)
            {
                /* ブレスの予備動作エフェクトを削除。*/
                if (pBreathEffect_)
                {
                    /* エフェクトを削除。*/
                    DeleteGO(pBreathEffect_);
                    pBreathEffect_ = nullptr;
                }

                /* 弾幕攻撃に移行する。*/
                currentStep_ = AttackStep::Barrage;
                stepTimer_ = 0.0f;
                shootTimer_ = 0.0f;

                /* 弾幕攻撃のアニメーションを再生。*/
                pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, true, 0.1f);
            }
        }


        void BossAttackFireBallState::UpdateBarrage(float dTime)
        {
            /* 弾幕攻撃中は一定間隔で火の玉を発射する。*/
            shootTimer_ += dTime;
            if (shootTimer_ >= SHOOT_INTERVAL)
            {
                /* 火の玉を発射する。*/
                shootTimer_ = 0.0f;
                ShotFireBall();
            }

            /* 弾幕攻撃が一定時間経過したら終了する。*/
            if (stepTimer_ >= BARRAGE_DURATION)
                currentStep_ = AttackStep::Finish;
        }


        void BossAttackFireBallState::LookAtPlayerDirection()
        {
            /* プレイヤーの方向を向く処理。*/
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                /* プレイヤーの位置とボスの位置の差分を計算。*/
                Vector3 diff = pPlayer->GetPlayerPos() - pBoss_->GetPos();
                diff.y = 0;
                diff.z = 0;

                /* 差分がある場合のみ回転を更新。*/
                if (diff.LengthSq() > 0.1f)
                {
                    /* ボスの回転をプレイヤーの方向に向ける。*/
                    rotation_.SetRotationYFromDirectionXZ(diff);
                    offSet_.SetRotationDegY(-90.0f);
                    pBoss_->SetRot(rotation_ * offSet_);
                }
            }
        }
    } 
} 
