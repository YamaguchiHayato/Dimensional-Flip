#include "stdafx.h"
#include "BossAttackRoar3DState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Stage/Gimmick/BossGimmick/RoarWave.h"

#include "Src/Core/SoundManager.h"
#include "Src/Core/BossUIManager.h"

namespace
{
    // 攻撃が発生するまでの時間。
    const auto ROAR_DURATION = 3.0f;

    // 攻撃が発生するタイミング。
    const auto SPAWN_TIMING = 1.0f;

    // 奇数推奨
    const int WAVE_COUNT = 5;

    // 広がる角度
    const float SPREAD_ANGLE = 15.0f;

    // 咆哮波の速度。
    const float WAVE_SPEED = 10.0f;
}

namespace app
{
    namespace enemyState
    {
        void BossAttackRoar3DState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;

            pBoss_->SetMoveSpeed(Vector3::Zero);

            // 咆哮モーション
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, false, 0.1f);

            // 攻撃タイプの設定。
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Roar3D);

            // 咆哮SEの再生。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Roar, 2.0f);

            // UIを設定。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Roar);

            // Playerの方向を向くように計算。
            UpdateLookAtPlayer();
        }


        void BossAttackRoar3DState::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();

            // 一定時間経過後に攻撃を発生。
            if (timer_ >= SPAWN_TIMING && !isAttackSpawned_)
            {
                CreateRoarWave();
                isAttackSpawned_ = true;
            }
        }


        void BossAttackRoar3DState::Exit()
        {
            // 咆哮後の隙は長め
            auto interval = 4.0f + (static_cast<float>(rand() % 21) / 10.0f);
            pBoss_->SettNextInterval(interval);

            // ステートを抜ける際に攻撃UIを削除。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Roar);
        }


        bool BossAttackRoar3DState::IsFinished() const
        {
            return timer_ >= ROAR_DURATION;
        }


        void BossAttackRoar3DState::CreateRoarWave()
        {
            // ボスの向きを取得する。
            bossPosition_ = pBoss_->GetPos();
            // ボスの角度を取得する。
            bossAngle_ = pBoss_->GetRot();
            // ボスの正面方向を計算する。
            forwardDirection_ = Vector3::Front;
            // ボスの正面方向をワールド座標に変換する。
            bossAngle_.Apply(forwardDirection_);

            for (int i = 0; i < WAVE_COUNT; ++i)
            {
                // 咆哮波のスポーン位置を計算する。
                angleOffset_ = (i - (WAVE_COUNT / 2)) * SPREAD_ANGLE;
                sqredAngle_.SetRotationDegY(angleOffset_);

                moveDirection_ = forwardDirection_;
                // 発射ベクトルを計算する。
                sqredAngle_.Apply(moveDirection_);

                // 生成。
                pRoarWave_ = NewGO<app::gimmick::RoarWave>(0);

                // ボスの少し前からスポーンさせる。
                spawnPosition_ = bossPosition_ + (moveDirection_ * 5.0f);
                spawnPosition_.y = 0.0f;

                // 咆哮波のパラメータをセット。
                pRoarWave_->SetParam(spawnPosition_, moveDirection_, WAVE_SPEED);
            }
        }


        void BossAttackRoar3DState::UpdateLookAtPlayer()
        {
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                // プレイヤーの位置を取得する。
                toPlayer_ = pPlayer->GetPlayerPos() - pBoss_->GetPos();
                toPlayer_.y = 0.0f;

                // ベクトルの長さを考慮して回転する。
                if (toPlayer_.LengthSq() > 0.001f)
                {
                    lookAtPlayerAngle_.SetRotationYFromDirectionXZ(toPlayer_);
                    pBoss_->SetRot(lookAtPlayerAngle_);
                }
            }
        }
    } 
} 
