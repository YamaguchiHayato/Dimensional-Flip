#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "BossAttackState.h"


namespace
{
    const auto ATTACK_SPAWN_TIME = 0.8f; // 何秒で攻撃オブジェクトを生成し始めるか。

    // 攻撃種類ごとのインターバル時間。
    // Cast。
    const auto ATTACK_INTERVAL_ROAR = 4.0f + (static_cast<float>(rand() % 21) / 10.0f);
    // Roar。
    const auto ATTACK_INTERVAL_CAST = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);


    // ランダムなインターバル時間を生成する。
    // Cast攻撃用。
    float GetRandomCastInterval() {
        return ATTACK_INTERVAL_CAST;
    }
    
    // Roar攻撃用。
    float GetRandomRoarInterval() {
        return ATTACK_INTERVAL_ROAR;
    }
}

namespace app
{
    namespace enemy
    {
        void BossAttackState::Enter()
        {
            // 変数を初期化。
            timer_ = 0.0f;
            isAttackSpawned_ = false;

            // 攻撃タイプにあわせてアニメーションを再生させる。
            if (pBoss_->GetAttackType() == AttackType::Roar)
                // 咆哮アニメーション。
                pBoss_->LoadAnimation(BossAnimation::bossAnim_AttackRoar, false, 0.1f);

            else
                // 隕石・槍攻撃のアニメーション。
                pBoss_->LoadAnimation(BossAnimation::bossAnim_AttackCast, false, 0.1f);
        }


        void BossAttackState::Update()
        {
            // 時間計測。
            timer_ += g_gameTime->GetFrameDeltaTime();
            // 現在の攻撃タイプを取得。
            currentAttackType_ = pBoss_->GetAttackType();

            // アニメーションの再生をチェックする。
            if (currentAttackType_ != AttackType::Roar)
            {
                // 設定秒数で攻撃オブジェクトを生成する。
                if (timer_ >= ATTACK_SPAWN_TIME && !isAttackSpawned_)
                {
                    // フラグで生成済みかチェックする。
                    isAttackSpawned_ = true;
                    // 攻撃オブジェクトを生成する。
                    pBoss_->SpawnGimmicks(currentAttackType_);
                }
            }

        }


        bool BossAttackState::RequestID(uint8_t& request)
        {
            // 攻撃アニメーションが再生し終われば遷移判定を行う。
            if (!pBoss_->IsPlayingAnimation())
            {
                // 咆哮。
                if (currentAttackType_ == AttackType::Roar)
                {
                    // 次の攻撃へのインターバルを設定。
                    pBoss_->SettNextInterval(ATTACK_INTERVAL_ROAR);
                    // 待機状態へ移行。
                    request = BossState::state_Idle;
                    
                    return true;
                }

                // 隕石・槍攻撃の場合。
                // 攻撃回数をカウントする。
                pBoss_->AddAttackCount();

                // 疲労状態かどうかをチェックする。
                if (pBoss_->IsTired())
                {
                    // 攻撃カウントをリセットして疲労ステートへ。
                    pBoss_->ResetAttackCount();
                    request = BossState::state_Tumble;
                    return true;
                }

                else
                {
                    // カウントの回数へ達していないなら通常状態へ戻る。
                    pBoss_->SettNextInterval(GetRandomCastInterval());
                    request = BossState::state_Idle;
                    return true;
                }
            }


            return false;
        }
    }
}
