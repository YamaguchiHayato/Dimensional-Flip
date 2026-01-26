#include "stdafx.h"
#include "BossDamageState.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

#include "Src/Core/BattlePhaseManager.h"

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
            // 1. まだ疲労モードに入っておらず、かつダメージアニメーションが終わった場合
            if (!istiredPlaying_ && !pBoss_->IsPlayingAnimation())
            {
                // 現在HPを取得。
                uint8_t currentHP = pBoss_->GetHP();

                // HPが残っている場合
                if (currentHP > 0)
                {
                    pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, true, 0.2f);

                    // フラグを立てて、Updateでのタイマー計測を開始させる
                    istiredPlaying_ = true;
                    timer_ = 0.0f;

                    // まだステートは遷移しない
                    return false;
                }

                // HPが0ならば、フェーズを進行させて終了。
                else if (currentHP == 0)
                {
                    // ボス戦フェーズを進行させる。
                    app::core::BattlePhaseManager::GetInstance()->AdvancePhase();

                    // フェーズが進行するとHPを回復させる。
                    pBoss_->SetHP(MAX_HP);

                    // 死亡させずに3DモードでのIdle状態へ移る。
                    pBoss_->SettNextInterval(INTERVAL);
                    request = app::enemyStatus::BossState::state_Idle;
                    return true;
                }
            }

            // 2. 疲労アニメーションを指定時間再生したらIdleに戻る
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
