#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Character/Player/PlayerFallState.h"


namespace
{
    // リスポーンするかを決めるライン。
    const auto DEAD_LINE = -500.0f;

    // 重力。
    const auto GRAVITY = 15.0f;

    //
    const auto FALL_GRAVITY_SCALE = 2.5f;

    // 最大落下速度。
    const auto MAX_FALL_SPEED = -1000.0f;
}


namespace app
{
    namespace state
    {
        void PlayerFallState::Enter()
        {
            // 落下アニメーションはないので空。
        }


        void PlayerFallState::Update()
        {
            // 重力をかける。
            ApplyGravity();

            // 移動を反映させる。
            pPlayer_->ApplyMovement();

            // 描画更新。
            pPlayer_->GetModelRender();

            // 落下したかチェックする。
            CheckRespawn();
        }


        void PlayerFallState::Exit() {}


        bool PlayerFallState::RequestID(uint8_t& request)
        {
            // リスポーンした or 地面に着地した場合。
            if (pPlayer_->GetCharacterController().IsOnGround())
            {
                // Idle状態へ遷移。
                request = EnPlayerState::enState_Idle;
                return true;
            }

            return false;
        }


        void PlayerFallState::ApplyGravity()
        {
            Vector3& speed = pPlayer_->GetMoveSpeed();

            // 落下中は重力を強くする。
            speed.y -= GRAVITY * FALL_GRAVITY_SCALE;

            // 落下速度を制限する。
            if (speed.y < MAX_FALL_SPEED)
                speed.y = MAX_FALL_SPEED;
        }


        void PlayerFallState::CheckRespawn()
        {
            // 現在のY座標がデッドゾーンより下かどうか判断する。
            if (pPlayer_->GetPlayerPos().y < DEAD_LINE)
            {
                // リスポーン処理。
                Respawn();
            }
        }


        void PlayerFallState::Respawn()
        {
            if (pPlayer_->GetPlayerPos().y < DEAD_LINE)
            {
                // 座標と物理のセット。
                pPlayer_->SetPlayerPos(pPlayer_->GetRespwanPos());

                // 回転軸をセットする。
                pPlayer_->SetRotation(pPlayer_->GetRespwanRot());

                // 移動速度をリセット。
                pPlayer_->GetMoveSpeed() = Vector3::Zero;

                // フラグの設定。
                pPlayer_->SetRespawnFlag(true);
            }
        }
    }
}


