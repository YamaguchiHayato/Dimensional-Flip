#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemyIdleState.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemy.h"


namespace
{
    auto DELTA_TIME = 1.0f / 60.0f;
}

namespace app
{
    namespace enemyState
    {
        void FallEnemyIdleState::Enter()
        {
            // タイマーをセット。
            auto initTime = -pFallEnemy_->GetStartUpDelay();
            // タイマー初期化。
            SettingTimer(initTime, false);
        }


        void FallEnemyIdleState::Update()
        {
            // モードを取得する。
            auto mode = app::collision::CollisionManager::GetInstance().GetCurrentMode();
            bool is2Dmode = (mode == app::collision::DimensionMode::dim2D);


            if (is2Dmode)
            {
                // Playerを取得する。
                 Player* pPlayer = pFallEnemy_->GetPlayer();

                
                if (pPlayer)
                {
                    // Playerと EnemyモデルのX座標を取得。
                    // Player。
                    auto playerPosX = pPlayer->GetPlayerPos().x;

                    auto triggerX = pFallEnemy_->GetTriggerX();
                    // Playerが指定したX座標より左側にいるなら起動しない。
                    if (playerPosX < triggerX)
                        return;
                }

                // 時間計測。
                timer_ += DELTA_TIME;

                // フラグを立てる。
                if (timer_ >= pFallEnemy_->GetWaitBottomTime())
                    isTimeOver_ = true;
            }


            else
            {
                // 値が負の値にならないように補正。
                if (timer_ < 0.0f)
                    timer_ = 0.0f;

                // カウントアップ。
                timer_ += DELTA_TIME;

                // 落下判定。
                // 待機時間を超えたかチェックする。
                if (timer_ >= pFallEnemy_->GetWaitTopTime())
                    isTimeOver_ = true;

            }

        }


        bool FallEnemyIdleState::RequestID(uint8_t& request)
        {
            if (isTimeOver_)
            {
                // 次の遷移へ要求する。
                request = static_cast<uint8_t>(app::enemy::FallEnemyState::state_Fall);
                return true;
            }
            return false;
        }
    }
}
