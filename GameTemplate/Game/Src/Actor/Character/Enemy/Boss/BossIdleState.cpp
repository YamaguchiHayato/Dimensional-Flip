#include "stdafx.h"
#include "BossIdleState.h"


namespace app
{
    namespace enemy
    {
        void BossIdleState::Enter()
        {
            // 待機アニメーションを再生。
            pBoss_->LoadAnimation(BossAnimation::bossAnim_Idle, true, 0.1f);
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
                // 攻撃の種類をランダムに決定する。
                uint8_t randomAttack = rand() % static_cast<int>(AttackType::Num);
                pBoss_->SetAttackType(static_cast<AttackType>(randomAttack));

                request = BossState::state_Attack;
                return true;
            }

            return false;
        }
    }
}
