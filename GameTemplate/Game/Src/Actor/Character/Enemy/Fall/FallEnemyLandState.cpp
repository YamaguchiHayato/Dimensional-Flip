#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Fall/FallEnemyLandState.h"

namespace
{
    const auto DELTA_TIME = 1.0f / 60.0f;
}

namespace app
{
    namespace enemy
    {
        void FallEnemyLandState::Enter()
        {
            timer_ = 0.0f;
            isTimeOver_ = false;
        }


        void FallEnemyLandState::Update()
        {
            timer_ += DELTA_TIME;

            if (timer_ >= pFallEnemy_->GetWaitBottomTime())
                isTimeOver_ = true;
        }


        bool FallEnemyLandState::RequestID(uint8_t& request)
        {
            if (isTimeOver_)
            {
                // 上昇ステートへ遷移
                request = static_cast<uint8_t>(FallEnemyState::state_Rise);
                return true;
            }
            return false;
        }
    } 
} 
