#include "stdafx.h"
#include "BossAttackRoar2DState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Thunder.h"
#include "Src/Core/SoundManager.h"

namespace
{
    const uint8_t MAX_WAVES = 3;              //! 左右スィープを繰り返す回数
    const uint8_t STRIKES_PER_WAVE = 7;       //! 1画面に並べる雷の数
    const auto RANDOM_STRIKE_DURATION = 3.0f; //! ランダム落雷の時間
    const auto TOTAL_ATTACK_DURATION = 6.0f;  //! 咆哮攻撃全体の時間
    const auto STRIKE_INTERVAL = 0.4f;        //! 雷が左から右へ流れる速度（間隔）
    const auto WAVE_DELAY = 1.0f;             //! セットごとの待機時間
    const auto STAGE_RANGE_X = 35.0f;         //! ステージ端の座標。
    const auto ROAR_DURATION = 3.0f;          //! 咆哮の時間
}

namespace app
{
    namespace enemyState
    {
        void BossAttackRoar2DState::Enter(app::enemy::Boss* pBoss)
        {
            /* ボスクラスを保持。*/
            pBoss_ = pBoss;

            /* アニメーションの再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_AttackRoar, true, 0.1f);

            /* 攻撃タイプの設定。*/
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Roar2D);

            /* 移動速度を設定。*/
            pBoss_->SetMoveSpeed(Vector3::Zero);

            /* Playerの方向を向く。*/
            LookAtPlayerDirection();

            /* SEの再生。*/
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Roar, 2.0f);
        }


        void BossAttackRoar2DState::Update()
        {
            /* 経過時間の更新。*/
            auto deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;
            spawnTimer_ += deltaTime;

            /* 雷エフェクトの挙動。*/
            ThunderEffectMovement();

            /* Playerの方向を向く。*/
            LookAtPlayerDirection();
        }


        void BossAttackRoar2DState::Exit()
        {
            /* 攻撃インターバルを設定。*/
            pBoss_->SettNextInterval(3.0f);

            /* SEの停止。*/
            app::core::SoundManager::GetInstance()->StopSE(GameSoundList_SE_Roar);
        }


        bool BossAttackRoar2DState::IsFinished() const
        {
            /* 咆哮攻撃の時間が経過したか判定。*/
            return timer_ >= ROAR_DURATION;
        }


        void BossAttackRoar2DState::LookAtPlayerDirection()
        {
            /* Playerの方向を向く。*/
            auto* pPlayer = pBoss_->GetPlayer();

            /* Playerが存在しない場合は処理を終了。*/
            if (!pPlayer)
                return;

            /* Playerの座標とBossの座標の差分を計算。*/
            auto diffX = pPlayer->GetPlayerPos().x - pBoss_->GetPos().x;

            /* Playerの方向を向くための回転を計算。*/
            Quaternion targetRot;

            /* Playerが右側にいる場合は左を向き、左側にいる場合は右を向く。*/
            if (diffX > 0.0f)
                targetRot.SetRotationDegY(-90.0f);
            else
                targetRot.SetRotationDegY(90.0f);

            /* Bossの回転を設定。*/
            pBoss_->SetRot(targetRot);
        }


        void BossAttackRoar2DState::ThunderEffectMovement()
        {
            /* 雷エフェクトの挙動を管理。*/
            if (waveCount_ < MAX_WAVES)
            {
                /* 生成時間が経過したか判定。*/
                if (spawnTimer_ >= STRIKE_INTERVAL)
                {
                    /* 生成時間をリセット。*/
                    SetSpawnTimer(0.0f);

                    /* 雷エフェクトの生成位置を設定。*/
                    SettingThunderEffect();
                }
            }

            else
                isMovingAttackStarted_ = true;
        }


        void BossAttackRoar2DState::SettingThunderEffect()
        {
            /* 生成位置を計算。*/
            auto startPos = -STAGE_RANGE_X;
            auto endPos = STAGE_RANGE_X;
            auto stepX = (endPos - startPos) / (STRIKES_PER_WAVE - 1);
            auto spawnX = startPos + (stepX * strikeIndex);

            /* 雷エフェクトを生成。*/
            auto* t = NewGO<app::gimmick::Thunder>(0);
            t->SetParam(Vector3(spawnX, 0.0f, 0.0f), app::gimmick::ThunderMode::Stationary, 5.0f, 0.4);

            /* 生成位置のインデックスを更新。*/
            strikeIndex++;

            /* 1セットの雷を生成し終えた場合は、ウェーブ数を増やし、生成時間をリセット。*/
            if (strikeIndex >= STRIKES_PER_WAVE)
            {
                strikeIndex = 0;
                waveCount_++;
                spawnTimer_ = -WAVE_DELAY;
            }
        }
    }
}
