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
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;
            isFinished_ = false;

            // 攻撃アニメーションを再生する。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, false, 0.1f);

            // 攻撃UIを通知する。
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Meteor);
        }


        void BossAttackMeteo2DState::Update()
        {
            // 経過時間を取得する。
            timer_ += g_gameTime->GetFrameDeltaTime();

            // 一定時間の猶予を与えて、隕石を一斉に生成する。
            if (timer_ >= SPAWN_DELAY && !isAttackSpawned_)
            {
                // 隕石を生成する。
                SpawnMeteoLine();

                // 生成フラグをリセット。
                isAttackSpawned_ = true;
            }

            // 隕石を生成後、アニメーションが再生終了していれば、終了判定とする。
            if (isAttackSpawned_ && !pBoss_->IsPlayingAnimation())
                isFinished_ = true;
        }


        void BossAttackMeteo2DState::Exit()
        {
            // 次の攻撃までのインターバルをセットする。
            pBoss_->SettNextInterval(3.0f);
        }


        void BossAttackMeteo2DState::SpawnMeteoLine()
        {
            // 隕石を生成し始める座標を設定する。
            auto startX = STAGE_LIMIT;
            // 生成終了座標を設定。
            auto endX = -STAGE_LIMIT;
            // 隕石の生成間隔を計算。
            auto stepX = (startX - endX) / (METEO_COUNT - 1);

            // 隕石の生成ループ。
            for (uint8_t i = 0; i < METEO_COUNT; ++i)
            {
                // X座標を計算
                float spawnX = endX + (stepX * i);

                // 1. 生成時の座標（上空で見える高さ）を設定
                Vector3 spawnPos(spawnX, METEO_SPAWN_HEIGHT, 0.0f);

                // 2. 着地目標地点（地面）を設定
                Vector3 targetPos(spawnX, 0.0f, 0.0f);

                auto meteo = NewGO<app::gimmick::Meteo>(0);

                // 隕石の初期位置を上空にセット（カメラに映る範囲）
                meteo->SetPosition(spawnPos);

                // 落下目標地点をセット
                meteo->SetTargetPos(targetPos);

                // パラメータ設定：(落下速度, FLOAT_DURATION秒間の空中待機)
                meteo->SetParams(6.0f, FLOAT_DURATION);
            }
        }
    }     
}
