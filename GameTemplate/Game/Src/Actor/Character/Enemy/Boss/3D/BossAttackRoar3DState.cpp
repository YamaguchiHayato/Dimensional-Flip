#include "stdafx.h"
#include "BossAttackRoar3DState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Stage/Gimmick/BossGimmick/RoarWave.h"

#include "Src/Core/SoundManager.h"
#include "Src/Core/BossUIManager.h"

namespace
{
    const auto ROAR_DURATION = 3.0f;  //! 咆哮の持続時間。
    const auto SPAWN_TIMING = 1.0f;   //! 咆哮波のスポーンタイミング。
    const int WAVE_COUNT = 5;         //! 咆哮波の数。
    const float SPREAD_ANGLE = 15.0f; //! 咆哮波の拡散角度。
    const float WAVE_SPEED = 10.0f;   //! 咆哮波の速度。
}

namespace app
{
    namespace enemyState
    {
        void BossAttackRoar3DState::Enter(app::enemy::Boss* pBoss)
        {
            /* ボスのポインタを保持。*/
            pBoss_ = pBoss;

            /* タイマーをリセット。*/
            pBoss_->SetMoveSpeed(Vector3::Zero);

            /* アニメーションを再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, false, 0.1f);

            /* 攻撃のタイプを設定。*/
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Roar3D);

            /* SEを再生。*/
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Roar, 2.0f);

            /* 攻撃のUIを設定。*/
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Roar);

            /* 角度を更新。*/
            UpdateLookAtPlayer();
        }


        void BossAttackRoar3DState::Update()
        {
            /* タイマーを取得。*/
            timer_ += g_gameTime->GetFrameDeltaTime();

            /* プレイヤーの方向を向く。*/
            if (timer_ >= SPAWN_TIMING && !isAttackSpawned_)
            {
                /* 咆哮波を生成。*/
                CreateRoarWave();
                isAttackSpawned_ = true;
            }
        }


        void BossAttackRoar3DState::Exit()
        {
            /* タイマーをリセット。*/
            auto interval = 4.0f + (static_cast<float>(rand() % 21) / 10.0f);
            pBoss_->SettNextInterval(interval);

            /* 攻撃のUIを設定。*/
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Roar);
        }


        bool BossAttackRoar3DState::IsFinished() const
        {
            /* 咆哮の持続時間を超えたら終了。*/
            return timer_ >= ROAR_DURATION;
        }


        void BossAttackRoar3DState::CreateRoarWave()
        {
            /* 座標を取得。*/
            bossPosition_ = pBoss_->GetPos();

            /* ボスの前方方向を取得。*/
            bossAngle_ = pBoss_->GetRot();

            /* ボスの前方方向を計算。*/
            forwardDirection_ = Vector3::Front;

            /* ボスの回転を適用。*/
            bossAngle_.Apply(forwardDirection_);

            /* 咆哮波を生成。*/
            for (int i = 0; i < WAVE_COUNT; ++i)
            {
                /* 咆哮波の角度を計算。*/
                angleOffset_ = (i - (WAVE_COUNT / 2)) * SPREAD_ANGLE;
                sqredAngle_.SetRotationDegY(angleOffset_);
                moveDirection_ = forwardDirection_;

                /* 角度を適用。*/
                sqredAngle_.Apply(moveDirection_);

                /* 咆哮波を生成。*/
                pRoarWave_ = NewGO<app::gimmick::RoarWave>(0);

                /* 咆哮波のスポーン位置を計算。*/
                spawnPosition_ = bossPosition_ + (moveDirection_ * 5.0f);
                spawnPosition_.y = 0.0f;

                /* 咆哮波のパラメータを設定。*/
                pRoarWave_->SetParam(spawnPosition_, moveDirection_, WAVE_SPEED);
            }
        }


        void BossAttackRoar3DState::UpdateLookAtPlayer()
        {
            /* プレイヤーのポインタを取得。*/
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                /* プレイヤーの方向を計算。*/
                toPlayer_ = pPlayer->GetPlayerPos() - pBoss_->GetPos();
                toPlayer_.y = 0.0f;

                /* プレイヤーの方向が有効な場合、ボスの回転を更新。*/
                if (toPlayer_.LengthSq() > 0.001f)
                {
                    lookAtPlayerAngle_.SetRotationYFromDirectionXZ(toPlayer_);
                    pBoss_->SetRot(lookAtPlayerAngle_);
                }
            }
        }
    } 
} 
