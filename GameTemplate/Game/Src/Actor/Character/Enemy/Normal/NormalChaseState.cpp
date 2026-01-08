#include "stdafx.h"

#include "NormalChaseState.h"
#include "Src/Actor/Character/Enemy/Normal/NormalEnemy.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto FLOAT_SPEED = 0.05f;
    const auto FLOAT_RANGE = 10.0f;
} 

namespace app
{
    namespace enemy
    {
        void NormalChaseState::Update()
        {
            MoveChase();
        }


        bool NormalChaseState::RequestID(uint8_t& request)
        {
            if (!pNormal_)
                return false;

            // 1. 踏まれたか (最優先)
            if (pNormal_->TryStomp())
            {
                request = static_cast<uint8_t>(NormalEnemyState::state_Crushed);
                return true;
            }

            // 2. 追跡条件から外れたら Idle へ戻る
            if (!pNormal_->CheckChaseCondition())
            {
                request = static_cast<uint8_t>(NormalEnemyState::state_Idle);
                return true;
            }

            return false;
        }


        void NormalChaseState::MoveChase()
        {
            if (!pNormal_)
                return;

            angle_ += FLOAT_SPEED;
            float y01 = (sinf(angle_) + 1.0f) * 0.5f;
            float yOffset = y01 * FLOAT_RANGE;

            Vector3 currentInitPos = pNormal_->GetInitPos();
            Vector3 currentPos = pNormal_->GetCurrentPos();
            currentPos.y = currentInitPos.y + yOffset;

            // --- 2. 追跡移動 (XZ) ---
            Player* pPlayer = pNormal_->GetPlayer();
            if (pPlayer)
            {
                Vector3 toPlayer = pPlayer->GetPlayerPos() - currentPos;
                toPlayer.y = 0.0f; // 高さは無視

                // 距離チェック用のパラメータ取得 (NormalEnemyのprivateメンバにfriendアクセス)
                float distSq = toPlayer.LengthSq();
                float stopRadius = pNormal_->stopRadius_; // ★friendなのでアクセス可
                float aggroRadius = pNormal_->aggroRadius_;
                float moveSpeed = pNormal_->moveSpeed_;
                float accel = pNormal_->accel_;

                Vector3 velocity = pNormal_->GetVelocity();
                Vector3 desiredVel = Vector3::Zero;

                // 停止距離より遠ければ近づく
                if (distSq > stopRadius * stopRadius)
                {
                    float dist = sqrtf(distSq);
                    toPlayer.Normalize();

                    // イージング（近づくほど減速）
                    float denom = (aggroRadius - stopRadius);
                    if (denom < 1.0f)
                        denom = 1.0f;
                    float t = (dist - stopRadius) / denom;
                    t = max(0.0f, min(t, 1.0f));

                    desiredVel = toPlayer * (moveSpeed * t * t);
                }

                // 慣性移動
                velocity += (desiredVel - velocity) * accel;
                pNormal_->SetVelocity(velocity);

                // 座標更新
                currentPos.x += velocity.x;
                currentPos.z += velocity.z;

                // 基準位置(InitPos)も連れて行く
                currentInitPos.x = currentPos.x;
                currentInitPos.z = currentPos.z;
                pNormal_->SetInitPos(currentInitPos);
            }

            // 最終適用
            pNormal_->SetCurrentPos(currentPos);
            pNormal_->UpdateRender();
        }
    }
} 
