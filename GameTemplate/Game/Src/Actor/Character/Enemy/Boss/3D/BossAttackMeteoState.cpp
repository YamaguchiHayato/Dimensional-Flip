#include "stdafx.h"
#include "BossAttackMeteoState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Meteo.h"


namespace
{
    const auto SPAWN_TIME = 0.8f; // 何秒で攻撃オブジェクトを生成し始めるか。
    const int SPAWN_NUM = 6;
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
        }

        bool BossAttackMeteoState::IsFinished() const
        {

            return !pBoss_->IsPlayingAnimation();
        }


        void BossAttackMeteoState::CreateMeteoAttack()
        {
            for (int i = 0; i < SPAWN_NUM; i++)
            {
                // 座標はBossに計算してもらう
                Vector3 spawnPos = pBoss_->GetRandomAttackPos();

                // 隕石生成
                auto speed = 2.5f + static_cast<float>(rand() % 5);
                auto delay = 0.2f + (static_cast<float>(rand() % 15) / 10.0f);

                auto meteo = NewGO<app::gimmick::Meteo>(0);
                meteo->SetName("meteo");
                meteo->SetTargetPos(spawnPos);
                meteo->SetParams(speed, delay);
            }
        }
    } 
}
