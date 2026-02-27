#include "stdafx.h"
#include "BossAttackRoar3DState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"



namespace app
{
    namespace enemyState
    {
        void BossAttackRoar3DState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;
            // 咆哮モーション
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackRoar, false, 0.1f);
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Roar3D);
        }

        void BossAttackRoar3DState::Update()
        {
        }


        void BossAttackRoar3DState::Exit()
        {
            // 咆哮後の隙は長め
            auto interval = 4.0f + (static_cast<float>(rand() % 21) / 10.0f);
            pBoss_->SettNextInterval(interval);
        }


        bool BossAttackRoar3DState::IsFinished() const
        {
            return !pBoss_->IsPlayingAnimation();
        }
    } 
} 
