#include "stdafx.h"
#include "BossAttackMeteoState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Core/BossUIManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Meteo.h"


namespace
{
    // 何秒で攻撃オブジェクトを生成し始めるか。
    const auto SPAWN_TIME = 0.8f;

    // 生成する数。
    const int SPAWN_NUM = 6;

    // 何秒間攻撃をするか。
    const auto ATTACK_DURATION = 2.0f;
}

namespace app
{
    namespace enemyState
    {
        void BossAttackMeteoState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;

            // 隕石攻撃アニメーションの再生。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackCast, false, 0.1f);

            // 攻撃タイプをセット。
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Meteor);

            // 攻撃UIをセット。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Meteor);
        }


        void BossAttackMeteoState::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();

            // 隕石の生成。
            if (timer_ >= SPAWN_TIME && !isAttackSpawned_ )
            {
                isAttackSpawned_ = true;

                CreateMeteoAttack();
            }
        }


        void BossAttackMeteoState::Exit()
        {
            // 次のインターバルをランダムに設定。
            auto interval = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);
            pBoss_->SettNextInterval(interval);

            // UIを非表示に。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Meteor);

            // ステートを抜ける際はSEを停止する。
            app::core::SoundManager::GetInstance()->StopSE(GameSoundList_SE_SpawnBlock);
        }


        bool BossAttackMeteoState::IsFinished() const
        {
            return timer_ >= ATTACK_DURATION;
        }


        void BossAttackMeteoState::CreateMeteoAttack()
        {
            // 隕石を落下させるSEを再生。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SpawnBlock, 2.0f);

            for (int i = 0; i < SPAWN_NUM; i++)
            {
                // 座標はBossに計算してもらう
                Vector3 targetPos = pBoss_->GetRandomAttackPos(); 
                Vector3 spawnPos = targetPos;
                spawnPos.y += 200.0f;

                // 隕石生成
                auto speed = 2.5f + static_cast<float>(rand() % 5);
                auto delay = 0.2f + (static_cast<float>(rand() % 15) / 10.0f);

                // 隕石クラスの生成。
                auto meteo = NewGO<app::gimmick::Meteo>(0);
                meteo->SetName("meteo");

                // 落下し始める座標のセット。
                meteo->SetPosition(spawnPos);
                // 落下し終わる座標をセット。
                meteo->SetTargetPos(targetPos);

                // 落下速度と警告時間を設定。
                meteo->SetParams(speed, delay);
            }
        }
    } 
}
