#include "stdafx.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/State/PlayerFallState.h"
#include "Src/Actor/Character/Player/Component/PlayerStateMachine.h"
#include "Src/Core/Game.h"

using nsApp::nsActor::nsCharacter::nsPlayer::EnPlayerState;

namespace
{
    // リスポーンするかを決めるライン。
    const auto DEAD_LINE = -100.0f;

    // 重力。
    const auto GRAVITY = 15.0f;

    //
    const auto FALL_GRAVITY_SCALE = 2.5f;

    // 最大落下速度。
    const auto MAX_FALL_SPEED = -1000.0f;

    // 空中での移動倍率。
    // デフォルトの移動速度の n(設定値)倍。
    const auto AIR_MOVE_RATE = 0.4f;

}


namespace nsApp
{
    namespace nsState
    {
        void PlayerFallState::Update()
        {
            // --- 追加：落下中も入力を更新 ---
            Vector3 stick;
            stick.x = g_pad[0]->GetLStickXF();
            stick.z = g_pad[0]->GetLStickYF() * -1.0f;
            pPlayer_->SetKeyDirection(stick);


            // 落下中でも多少動けるように移動計算を追加。
            AirMovement(AIR_MOVE_RATE);

            // 重力をかける。
            ApplyGravity();

            pPlayer_->ApplyMovement();

            CheckRespawn();
        }


        bool PlayerFallState::RequestID(uint8_t& request)
        {
            // リスポーンした or 地面に着地した場合。
            if (pPlayer_->GetCharacterController().IsOnGround()|| pPlayer_->IsRespawn())
            {
                // Idle状態へ遷移。
                request = EnPlayerState::enState_Idle;
                return true;
            }


            return false;
        }


        void PlayerFallState::AirMovement(float moveRate)
        {
            const Vector3& keyDir = pPlayer_->GetKeyDirection();
            Vector3& moveSpeed = pPlayer_->GetMoveSpeed();

            // Player.h の walkSpeed_ を利用して空中移動速度を計算
            moveSpeed.x = keyDir.x * pPlayer_->GetWalkSpeed() * moveRate;
            moveSpeed.z = keyDir.z * pPlayer_->GetWalkSpeed() * moveRate;
        }


        void PlayerFallState::ApplyGravity()
        {
            Vector3& speed = pPlayer_->GetMoveSpeed();
            const bool isGround = pPlayer_->GetCharacterController().IsOnGround();
            const auto& air = nsApp::nsSystem::PlayerAirParameterTable::Get();

            bool isBounce = pPlayer_->IsBounce();
            pPlayer_->locomotion_.ApplyAirGravity(speed, air, isGround, isBounce, true);
            pPlayer_->SetIsBounce(isBounce);
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
            auto* pGame = FindGO<app::core::Game>("game");
            if (pGame && pGame->IsStageTransitioning())
                return;

            if (pPlayer_->GetPlayerPos().y < DEAD_LINE)
            {
                // ダメージを1受ける。
                pPlayer_->OnDamage(1);

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


