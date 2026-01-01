#include "stdafx.h"
#include "NormalIdleState.h"
#include "Src/Actor/Character/Enemy/Normal/NormalEnemy.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto FLOAT_SPEED = 0.05f;
    const auto FLOAT_RAGE = 10.0f;
}


namespace app
{
    namespace enemy
    {
        void NormalIdleState::Enter()
        {

        }


        void NormalIdleState::Update()
        {
            // 浮遊移動処理（3D時のみ追従）
            MoveFloating();

        }


        void NormalIdleState::Exit()
        {

        }


        bool NormalIdleState::RequestID(uint8_t& request)
        {
            if (!pNormal_)
                return false;

            // 踏まれたかどうか確認する。
            if (pNormal_->TryStomp())
            {
                request = static_cast<uint8_t>(NormalEnemyState::state_Crushed);
                return true;
            }

            // 2. 追跡条件チェック (これが無いと追いかけてきません)
            // 3Dモード かつ 範囲内なら
            if (pNormal_->CheckChaseCondition())
            {
                request = static_cast<uint8_t>(NormalEnemyState::state_Chase);
                return true;
            }


            return false;
        }


        void NormalIdleState::MoveFloating()
        {
            if (!pNormal_)
                return;

            // --- 上下の浮遊（Y）---
            angle_ += FLOAT_SPEED;


            float y01 = (sinf(angle_) + 1.0f) * 0.5f;
            float yOffset = y01 * FLOAT_RAGE;


            // 基準位置を取得。
            Vector3 currentPos = pNormal_->GetInitPos();
            Vector3 newPos = pNormal_->GetCurrentPos();
            newPos.y = currentPos.y + yOffset;


            // 速度減衰の処理。
            Vector3 velocity = pNormal_->GetVelocity();
            velocity.x *= 0.8f;
            velocity.z *= 0.8f;
            pNormal_->SetVelocity(velocity);


            // 位置更新（XZのみ）
            newPos.x += velocity.x;
            newPos.z += velocity.z;


            // InitPosのXZに追従させる。
            currentPos.x = newPos.x;
            currentPos.z = newPos.z;
            pNormal_->SetInitPos(currentPos);


            // 座標を更新。
            pNormal_->SetCurrentPos(newPos);

            pNormal_->UpdateRender();

        }

    }
}
