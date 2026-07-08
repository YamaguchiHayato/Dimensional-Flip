#include "stdafx.h"
#include "BossAttackMeteoState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Core/SoundManager.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Meteo.h"
#include "Src/Presentation/UI/BossAttackHudHelper.h"

namespace
{
    const auto SPAWN_TIME = 0.8f;      //! 隕石の生成タイミング。
    const int SPAWN_NUM = 6;           //! 隕石の生成数。
    const auto ATTACK_DURATION = 2.0f; //! 攻撃の持続時間。
}

namespace app
{
    namespace enemyState
    {
        void BossAttackMeteoState::Enter(app::enemy::Boss* pBoss)
        {
            /* 初期化。*/
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;

            /* アニメーションの再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackCast, false, 0.1f);

            /* 攻撃タイプのセット。*/
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Meteor);

            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Meteor);
        }


        void BossAttackMeteoState::Update()
        {
            /* タイマーの加算。*/
            timer_ += g_gameTime->GetFrameDeltaTime();

            /* 隕石の生成。*/
            if (timer_ >= SPAWN_TIME && !isAttackSpawned_ )
            {
                // 隕石の生成は一度だけ。
                isAttackSpawned_ = true;
                CreateMeteoAttack();
            }
        }


        void BossAttackMeteoState::Exit()
        {
            /* インターバルを設定。*/
            auto interval = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);
            pBoss_->SettNextInterval(interval);

            /* 隕石の生成SEを停止。*/
            app::core::SoundManager::GetInstance()->StopSE(GameSoundList_SE_SpawnBlock);

            /* 攻撃タイプのリセット。*/
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Meteor);
        }


        bool BossAttackMeteoState::IsFinished() const
        {
            /* 攻撃の持続時間を超えたら終了。*/
            return timer_ >= ATTACK_DURATION;
        }


        void BossAttackMeteoState::CreateMeteoAttack()
        {
            /* 隕石の生成SEを再生。*/
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SpawnBlock, 2.0f);

            /* 隕石の生成。*/
            for (int i = 0; i < SPAWN_NUM; i++)
            {
                /* 座標を取得。*/
                Vector3 targetPos = pBoss_->GetRandomAttackPos(); 
                Vector3 spawnPos = targetPos;
                spawnPos.y += 200.0f;

                /* 落下速度と警告時間をランダムに設定。*/
                auto speed = 2.5f + static_cast<float>(rand() % 5);
                auto delay = 0.2f + (static_cast<float>(rand() % 15) / 10.0f);

                /* 隕石の生成。*/
                auto meteo = NewGO<app::gimmick::Meteo>(0);
                meteo->SetName("meteo");

                /* 座標をセット。*/
                meteo->SetPosition(spawnPos);
                meteo->SetTargetPos(targetPos);
                meteo->SetParams(speed, delay);
            }
        }
    } 
}
