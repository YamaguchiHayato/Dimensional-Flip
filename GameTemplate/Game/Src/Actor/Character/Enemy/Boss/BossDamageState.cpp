#include "stdafx.h"
#include "BossDamageState.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app
{
    namespace enemyState
    {
        void BossDamageState::Enter()
        {
            // ダメージアニメーションを再生する。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Hit, false, 0.1f);
        }


        bool BossDamageState::RequestID(uint8_t& request)
        {
            // アニメーションが再生し終われば遷移。
            if (!pBoss_->IsPlayingAnimation())
            {
                // HPが0になれば死亡。
                if (pBoss_->hp <= 0)
                    return false;


                // 生きているなら待機状態へ戻る。
                pBoss_->SettNextInterval(3.0f);
                request = app::enemyStatus::BossState::state_Idle;
                return true;
            }
            return false;
        }
    }
}
