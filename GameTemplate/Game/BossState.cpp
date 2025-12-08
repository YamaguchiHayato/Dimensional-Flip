#include "stdafx.h"
#include "BossState.h"
#include "Src/Actor/Character/Enemy/Boss.h"


namespace app
{
    namespace state
    {
        BossState::BossState(Boss* pBoss) : pBoss_(pBoss) {}

        // 待機ステートのコンストラクタ。
        BossIdleState::BossIdleState(Boss* pBoss) : BossState(pBoss){}


        void BossIdleState::Enter(Boss* actor)
        {
            actor->Play
        }
    }
}
