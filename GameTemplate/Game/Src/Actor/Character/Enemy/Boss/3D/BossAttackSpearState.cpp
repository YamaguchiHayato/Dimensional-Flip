#include "stdafx.h"

#include "BossAttackSpearState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Stage/Gimmick/Spear.h" // 槍クラス

namespace
{
    const float SPAWN_TIME = 0.8f;
    const int SPAWN_NUM = 6; 
} 

namespace app
{
    namespace enemyState
    {
        void BossAttackSpearState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;

            // 槍も詠唱モーションを使用
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackCast, false, 0.1f);
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Spear);
        }


        void BossAttackSpearState::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();

            if (timer_ >= SPAWN_TIME && !isAttackSpawned_ )
            {
                isAttackSpawned_ = true;

                // 槍攻撃の生成。
                CreateSpearAttack();
            }
        }


        void BossAttackSpearState::Exit()
        {
            // インターバル設定
            auto interval = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);
            pBoss_->SettNextInterval(interval);
        }


        void BossAttackSpearState::CreateSpearAttack()
        {
            // 槍生成ループ
            for (int i = 0; i < SPAWN_NUM; i++)
            {
                Vector3 spawnPos = pBoss_->GetRandomAttackPos();

                // 槍の生成 (パラメータ設定なし)
                auto spear = NewGO<app::gimmick::Spear>(0);
                spear->SetName("spear");
                spear->SetTargetPos(spawnPos);
            }

        }

    } 
} 
