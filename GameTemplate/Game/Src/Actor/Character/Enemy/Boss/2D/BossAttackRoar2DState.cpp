#include "stdafx.h"
#include "BossAttackRoar2DState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Thunder.h"

namespace
{
    const auto RANDOM_STRIKE_DURATION = 3.0f; // ランダム落雷の時間
    const auto TOTAL_ATTACK_DURATION = 6.0f;  // 咆哮攻撃全体の時間

    const uint8_t MAX_WAVES = 3;              // 左右スィープを繰り返す回数
    const uint8_t STRIKES_PER_WAVE = 7;       // 1画面に並べる雷の数
    const auto STRIKE_INTERVAL = 0.4f;        // 雷が左から右へ流れる速度（間隔）
    const auto WAVE_DELAY = 1.0f;             // セットごとの待機時間

    const auto STAGE_RANGE_X = 35.0f;         // ステージ端の座標。


}

namespace app
{
    namespace enemyState
    {
        void BossAttackRoar2DState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;

            // 咆哮アニメーションを設定する。
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_AttackRoar, false, 0.1f);

            // 攻撃タイプの設定。
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Roar2D);

            // 移動速度を初期化する。
            pBoss_->SetMoveSpeed(Vector3::Zero);

            // Playerの方を向かせるように補正。
            LookAtPlayerDirection();
        }


        void BossAttackRoar2DState::Update()
        {
            // 経過時間を取得。
            auto deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;
            spawnTimer_ += deltaTime;

            // 雷エフェクトの挙動。
            ThunderEffectMovement();

            // 咆哮中もPlayerの方向を向いてほしいので、補正。
            LookAtPlayerDirection();
        }


        void BossAttackRoar2DState::Exit()
        {
            // 咆哮後のインターバルを設定。
            pBoss_->SettNextInterval(3.0f);
        }


        bool BossAttackRoar2DState::IsFinished() const
        {
            return isMovingAttackStarted_ && !pBoss_->IsPlayingAnimation();
        }


        void BossAttackRoar2DState::LookAtPlayerDirection()
        {
            // 初期のボスモデルの向きを設定する。(2DなのでX軸をあわせる。)
            auto* pPlayer = pBoss_->GetPlayer();

            if (!pPlayer)
                return;

            // X座標の差分を計算。
            auto diffX = pPlayer->GetPlayerPos().x - pBoss_->GetPos().x;

            Quaternion targetRot;

            if (diffX > 0.0f)
                targetRot.SetRotationDegY(-90.0f);

            else
                targetRot.SetRotationDegY(90.0f);

            // 回転を合成して適応。
            pBoss_->SetRot(targetRot);
        }


        void BossAttackRoar2DState::ThunderEffectMovement()
        {
            // フェーズ1
            // 左から順番に落としていく。
            if (waveCount_ < MAX_WAVES)
            {
                if (spawnTimer_ >= STRIKE_INTERVAL)
                {
                    // 生成時間を設定。
                    SetSpawnTimer(0.0f);

                    // エフェクトの位置を設定。
                    SettingThunderEffect();
                }
            }

            else
            {
                isMovingAttackStarted_ = true;
            }
        }


        void BossAttackRoar2DState::SettingThunderEffect()
        {
            // ステージ端(-35 ～　35)。
            // エフェクトの位置を均等に分割。
            auto startPos = -STAGE_RANGE_X;
            auto endPos = STAGE_RANGE_X;
            auto stepX = (endPos - startPos) / (STRIKES_PER_WAVE - 1);

            auto spawnX = startPos + (stepX * strikeIndex);


            // エフェクトの生成。
            auto* t = NewGO<app::gimmick::Thunder>(0);
            t->SetParam(Vector3(spawnX, 0.0f, 0.0f), app::gimmick::ThunderMode::Stationary, 5.0f, 0.4);

            // 次の位置に移動する。
            strikeIndex++;

            // 1画面分を出し切ったら次のフェーズへ。
            if (strikeIndex >= STRIKES_PER_WAVE)
            {
                strikeIndex = 0;
                waveCount_++;
                spawnTimer_ = -WAVE_DELAY;
            }
        }
    }
}
