#include "stdafx.h"
#include "BossDamageState.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

#include "Src/Core/BattlePhaseManager.h"
#include "Src/Core/SoundManager.h"

namespace
{
    const auto RECOVERY_TIME = 1.5f; // ダメージ後の回復時間。

    const auto MAX_HP = 3.0f; // ボスの最大HP。

    const auto INTERVAL = 3.0f; // 攻撃インターバル時間。
}

namespace app
{
    namespace enemyState
    {
        void BossDamageState::Enter()
        {
            // ダメージSE
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_BossHit, 3.0f);

            // 念のため、ここではPlayerのジャンプSEは停止する。
            app::core::SoundManager::GetInstance()->StopSE(GameSoundList_SE_Player_Jump);

            // ダメージアニメーションを再生する。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Hit, false, 0.1f);

            auto currentHP = pBoss_->GetHP();

            // ボスのHPを減らす。
            if (currentHP > 0)
                pBoss_->SetHP(currentHP - 1.0f);

            // 初期化。
            istiredPlaying_ = false;
            timer_ = 0.0f;

        }


        void BossDamageState::Update()
        {
            // 疲労アニメ再生中ならタイマーを進める
            if (istiredPlaying_)
                timer_ += g_gameTime->GetFrameDeltaTime();
        }


        bool BossDamageState::RequestID(uint8_t& request)
        {
            // ダメージアニメーションが終わったら判定を行う
            if (!istiredPlaying_ && !pBoss_->IsPlayingAnimation())
            {
                // 現在のHPを取得
                uint8_t currentHP = pBoss_->GetHP();

                // HPが0なら死亡ステートへ
                if (currentHP <= 0)
                {
                    request = app::enemyStatus::BossState::state_Dead;
                    return true;
                }

                // HPが残っているなら転倒状態へ
                else
                {
                    pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, true, 0.2f);
                    istiredPlaying_ = true;
                    timer_ = 0.0f;
                    return false;
                }
            }

            // 転倒（疲労）アニメーション中の復帰処理
            if (istiredPlaying_)
            {
                if (timer_ >= RECOVERY_TIME)
                {
                    pBoss_->SettNextInterval(INTERVAL);
                    request = app::enemyStatus::BossState::state_Idle;
                    return true;
                }
            }

            return false;
        }
    }
}
