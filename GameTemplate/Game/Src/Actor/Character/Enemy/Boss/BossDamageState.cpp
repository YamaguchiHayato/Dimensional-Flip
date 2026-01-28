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
            uint8_t currentHP = pBoss_->GetHP();
            auto* pPhase = app::core::BattlePhaseManager::GetInstance()->GetCurrentPhase();

            if (!istiredPlaying_ && !pBoss_->IsPlayingAnimation())
            {
                // 1. まず「フェーズ3」での死亡を最優先でチェック
                if (*pPhase == app::enemyStatus::BossPhase::phase_Three && currentHP == 0)
                {
                    request = app::enemyStatus::BossState::state_Dead;
                    return true;
                }

                // 2. まだHPがあるなら転倒（ダウン）へ
                if (currentHP > 0)
                {
                    pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, true, 0.2f);
                    istiredPlaying_ = true;
                    timer_ = 0.0f;
                    return false;
                }

                // 3. HPが0で、かつフェーズ3以外なら、フェーズを進めて回復（次のフェーズのIdleへ）
                else
                {
                    app::core::BattlePhaseManager::GetInstance()->AdvancePhase();
                    pBoss_->SetHP(MAX_HP);
                    pBoss_->SettNextInterval(INTERVAL);
                    request = app::enemyStatus::BossState::state_Idle;
                    return true;
                }
            }

            // 疲労アニメーション中の復帰タイマー処理
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
