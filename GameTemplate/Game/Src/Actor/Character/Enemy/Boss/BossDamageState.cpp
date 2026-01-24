#include "stdafx.h"
#include "BossDamageState.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

#include "Src/Core/BattlePhaseManager.h"

namespace
{
    const auto RECOVERY_TIME = 1.5f; // ダメージ後の回復時間。
}

namespace app
{
    namespace enemyState
    {
        void BossDamageState::Enter()
        {
            // ダメージアニメーションを再生する。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Hit, false, 0.1f);

            // ボスのHPを減らす。
            if (pBoss_->hp > 0)
                pBoss_->hp--;

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
                // HPが残っている場合
                if (pBoss_->hp > 0)
                {
                    // ★修正ポイント: すぐにIdleに戻らず、一度「疲労アニメーション」に切り替える
                    pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, true, 0.2f);

                    // フラグを立てて、Updateでのタイマー計測を開始させる
                    istiredPlaying_ = true;
                    timer_ = 0.0f;

                    // まだステートは遷移しない
                    return false;
                }

                // HPが0ならば、フェーズを進行させて終了。
                else if (pBoss_->hp == 0)
                {
                    // ボス戦フェーズを進行させる。
                    app::core::BattlePhaseManager::GetInstance()->AdvancePhase();

                    // フェーズが進行するとHPを回復させる。
                    pBoss_->hp = 3;

                    // 死亡させずに3DモードでのIdle状態へ移る。
                    pBoss_->SettNextInterval(3.0f);
                    request = app::enemyStatus::BossState::state_Idle;
                    return true;
                }
            }

            // 2. 疲労アニメーションを指定時間再生したらIdleに戻る
            if (istiredPlaying_)
            {
                if (timer_ >= RECOVERY_TIME)
                {
                    pBoss_->SettNextInterval(3.0f);
                    request = app::enemyStatus::BossState::state_Idle;

                    return true;
                }
            }

            return false;
        }
    }
}
