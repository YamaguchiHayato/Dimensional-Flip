#include "stdafx.h"
#include "BossIdleState.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app
{
    namespace enemyState
    {
        void BossIdleState::Enter()
        {
            // 待機アニメーションを再生。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Idle, true, 0.1f);
            timer_ = 0.0f;
        }


        void BossIdleState::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();
        }


        bool BossIdleState::RequestID(uint8_t& request)
        {
            // インターバル時間を超えると攻撃開始。
            if (timer_ >= pBoss_->GetNextInterval())
            {
                // 攻撃ステートへ遷移。
                request = app::enemyStatus::BossState::state_Attack;
                return true;
            }

            return false;
        }
    }
}
