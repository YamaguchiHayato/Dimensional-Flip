#include "stdafx.h"
#include "BossAttackMeteo2DState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Meteo.h"

namespace
{
    const auto SPAWN_DELAY = 1.2f;         // 咆哮開始から落下するまでの猶予。
    const auto FLOAT_DURATION = 3.0f;      // 隕石が落下するまでの時間。
    const uint8_t METEO_COUNT = 7;         // 落下する隕石の数。
    const auto STAGE_LIMIT = 35.0f;        // カメラに映るステージ端の座標。
    const auto METEO_SPAWN_HEIGHT = 35.0f; // 隕石の生成高さ。
}

namespace app
{
    namespace enemyState
    {
        void BossAttackMeteo2DState::Enter(app::enemy::Boss* pBoss)
        {
            /* 初期化。*/
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;
            isFinished_ = false;

            /* 攻撃アニメーションの再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, false, 0.1f);

            /* 攻撃タイプを設定。*/
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Meteor);
        }


        void BossAttackMeteo2DState::Update()
        {
            /* 経過時間を取得。*/
            timer_ += g_gameTime->GetFrameDeltaTime();

            /* 攻撃アニメーションが再生中で、かつ、隕石がまだ生成されていない場合。*/
            if (timer_ >= SPAWN_DELAY && !isAttackSpawned_)
            {
                /* 隕石を生成。*/
                SpawnMeteoLine();

                /* 隕石生成済みフラグを立てる。*/
                isAttackSpawned_ = true;
            }

            /* 攻撃アニメーションが終了した場合、状態を終了する。*/
            if (isAttackSpawned_ && !pBoss_->IsPlayingAnimation())
                isFinished_ = true;
        }


        void BossAttackMeteo2DState::Exit()
        {
            /* 次の攻撃までのインターバルを設定。*/
            pBoss_->SettNextInterval(3.0f);
        }


        void BossAttackMeteo2DState::SpawnMeteoLine()
        {
            /* 生成開始地点を設定。*/
            auto startX = STAGE_LIMIT;
            /* 生成終了地点を設定。*/
            auto endX = -STAGE_LIMIT;
            /* 生成間隔を計算。*/
            auto stepX = (startX - endX) / (METEO_COUNT - 1);

            /* 隕石を生成。*/
            for (uint8_t i = 0; i < METEO_COUNT; ++i)
            {
                /* 生成座標を計算。*/
                float spawnX = endX + (stepX * i);

                /* 隕石を上空に設定。*/
                Vector3 spawnPos(spawnX, METEO_SPAWN_HEIGHT, 0.0f);

                /* 隕石の落下目標地点を設定。*/
                Vector3 targetPos(spawnX, 0.0f, 0.0f);

                /* 隕石を生成。*/
                auto meteo = NewGO<app::gimmick::Meteo>(0);

                /* 隕石の座標を設定。*/
                meteo->SetPosition(spawnPos);

                /* 隕石の落下目標地点を設定。*/
                meteo->SetTargetPos(targetPos);

                /* 隕石の落下速度と落下までの猶予時間を設定。*/
                meteo->SetParams(6.0f, FLOAT_DURATION);
            }
        }
    }     
}
