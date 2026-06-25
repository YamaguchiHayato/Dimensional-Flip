#include "stdafx.h"

#include "PlayerIdleState.h"
#include "Src/Actor/Character/Player/Component/PlayerStateMachine.h"

using nsApp::nsActor::nsCharacter::nsPlayer::EnPlayerState;

namespace nsApp
{
    namespace nsState
    {
        void PlayerIdleState::Enter()
        {
            /** @brief Index をセット。 */
            context_.SetCurrentIndex(0);

            /** @brief 着地時にフラグをリセット。 */
            context_.SetRespawnFlag(false);
        }


        void PlayerIdleState::Update()
        {
            if (context_.GetKeyDirection().x < 0.0f)
                context_.SetCurrentIndex(0);
            else
                context_.SetCurrentIndex(4);

            Vector3& speed = context_.GetMoveSpeed();
            speed.x = 0.0f;
            speed.z = 0.0f;

            /** @brief 座標と描画の同期。 */
            context_.ApplyMovement();
            context_.SyncView();
        }


        bool PlayerIdleState::RequestID(uint8_t& request)
        {
            /** @brief 地面についていたら。 */
            if (context_.IsOnGround())
            {
                /** @brief A ボタンを押した瞬間だけジャンプ開始。 */
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    request = EnPlayerState::enState_Jump;
                    return true;
                }

                auto speedX = g_pad[0]->GetLStickXF() * 1.0f * 10.0f;
                auto speedY = g_pad[0]->GetLStickYF() * -1.0f * 10.0f;

                /**
                 * @brief x / z の移動速度があったらスティック入力とみなす。
                 */
                if (fabsf(speedX) >= 0.02f || fabsf(speedY) >= 0.02f)
                {
                    request = EnPlayerState::enState_Run;
                    return true;
                }
            }
            else
            {
                request = EnPlayerState::enState_Fall;
                return true;
            }

            return false;
        }
    } // namespace nsState
} // namespace nsApp
