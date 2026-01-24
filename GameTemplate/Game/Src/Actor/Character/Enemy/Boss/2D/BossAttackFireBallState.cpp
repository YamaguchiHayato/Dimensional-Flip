#include "stdafx.h"
#include "BossAttackFireBallState.h"

#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto STATE_DURATION = 2.5f;   // 攻撃継続時間
    const auto SHOOT_INTERVAL = 0.8f;   // 発射間隔（秒）
    const auto FIREBALL_SPEED = 400.0f; // 火の玉の速さ
                                        
    const auto SPREAD_ANGLE = 45.0f; // ばらつかせる角度。
    const auto PI = 3.1415926f;
}

namespace app
{
    namespace enemyState
    {
        void BossAttackFireBallState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;
            timer_ = 0.0f;
            shootTimer_ = 0.0f;

            // 咆哮アニメーションの再生。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, true, 0.1f);

            // 移動速度を初期化。
            pBoss_->SetMoveSpeed(Vector3::Zero);
        }


        void BossAttackFireBallState::Update()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;
            shootTimer_ += deltaTime;

            // 発射中もPlayerの方を向く。
            LookAtPlayerDirection();

            // 一定間隔ごとに発射。
            if (shootTimer_ >= SHOOT_INTERVAL)
            {
                shootTimer_ -= SHOOT_INTERVAL;

                // エフェクトを生成する。
                ShotFireBall();
            }
        }


        void BossAttackFireBallState::Exit()
        {
            // 次の攻撃までのインターバル。
            pBoss_->SettNextInterval(2.0f);
        }


        bool BossAttackFireBallState::IsFinished() const
        {
            return timer_ >= STATE_DURATION;
        }


        void BossAttackFireBallState::ShotFireBall()
        {
            // 発射位置を調整。
            spawnPos_ = pBoss_->GetPos();
            spawnPos_.y += 50.0f;


            // 火の玉を生成
            pFireBall_ = NewGO<app::gimmick::FireBall>(0);

            // 火の玉の射出方向を計算。
            CalculateFireBallDirection();

            // パラメータ設定 (位置、方向、速度)
            pFireBall_->SetParameter(spawnPos_, pPlayer, FIREBALL_SPEED);
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

                    Quaternion finalRot = rotation_ * offSet_;
                    pBoss_->SetRot(finalRot);
                }
            }
        }


        void BossAttackFireBallState::CalculateFireBallDirection()
        {
            // Bossクラス経由でPlayerを取得。
            auto* pPlayer = pBoss_->GetPlayer();

            if (pPlayer)
            {
                // PlayerとBossクラスのベクトルを取得。
                Vector3 toPlayer = pPlayer->GetPlayerPos() - spawnPos_;
                toPlayer.z = 0.0f;

                // 現在の角度を計算する。
                currentAngle_ = atan2f(toPlayer.y, toPlayer.x);

                // ランダムな射出角度を付ける。
                // 0.5…乱数の補正値。
                randomAngle_ = (static_cast<float>(rand() % 100 / 100.0f)) - 0.5f;

                // 角度のオフセット。
                angleOffset_ = randomAngle_ * SPREAD_ANGLE * (PI / 180.0f);

                // 最終的な角度を計算する。
                finalAngle_ = currentAngle_ + angleOffset_;

                // 角度からベクトルに変換する。
                targetDir.x = cosf(finalAngle_);
                targetDir.y = sinf(finalAngle_);
                targetDir.z = 0.0f;
            }


        }
    }
}
