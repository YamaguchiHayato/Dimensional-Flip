#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemyFallState.h"
#include "Src/Core/SoundManager.h"
namespace
{
    const float DELTA_TIME = 1.0f / 60.0f;
}

namespace app
{
    namespace enemyState
    {

        void FallEnemyFallState::Enter()
        {
            isGround_ = false;
            // 落下時の効果音再生。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Fall);

        }

        void FallEnemyFallState::Update()
        {
            if (!pFallEnemy_)
                return;

            // 現在座標取得
            Vector3 pos = pFallEnemy_->GetPosition();

            // 開始位置取得
            Vector3 startPos = pFallEnemy_->GetStartPos();

            // 移動処理 (下方向へ)
            pos.y -= pFallEnemy_->GetFallSpeed() * DELTA_TIME;

            // X座標を固定。
            pos.x = startPos.x;

            // Z座標を固定。
            pos.z = startPos.z;

            // 床判定。
            auto groundY = 0.0f; // 仮の床のY座標。
            // 到達判定
            if (pos.y <= groundY)
            {
                pos.y = groundY; // 行き過ぎないように補正
                isGround_ = true;
            }

            pFallEnemy_->SetPos(pos);
        }


        bool FallEnemyFallState::RequestID(uint8_t& request)
        {
            if (isGround_)
            {
                // 着地待機へ遷移
                request = static_cast<uint8_t>(app::enemy::FallEnemyState::state_Land);
                return true;
            }
            return false;
        }
    } 
} 
