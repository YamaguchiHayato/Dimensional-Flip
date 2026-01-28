#include "stdafx.h"
#include "BossDeadState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"

namespace app
{
    namespace enemyState
    {
        void BossDeadState::Enter()
        {
            // 死亡アニメーションを再生する。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Dead, false, 0.2f);

            if (pBoss_->pWeeekPoint_)
                // 攻撃ポイントを無効化する。
                pBoss_->pWeeekPoint_->SetIsEnable(false);
        }


        void BossDeadState::Update()
        {
            // @ TODO アニメーションが終わればエンディングへ。
        }


        void BossDeadState::Exit()
        {

        }


        bool BossDeadState::RequestID(uint8_t& request)
        {
            return false;
        }
    }
}
