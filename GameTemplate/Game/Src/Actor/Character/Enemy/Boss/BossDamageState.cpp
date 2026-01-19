#include "stdafx.h"
#include "BossDamageState.h"


namespace app
{
    namespace enemy
    {
        void BossDamageState::Enter()
        {
            // ダメージアニメーションを再生する。
            pBoss_->LoadAnimation(BossAnimation::bossAnim_Hit, false, 0.1f);
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
                request = BossState::state_Idle;
                return true;
            }
            return false;
        }
    }
}
