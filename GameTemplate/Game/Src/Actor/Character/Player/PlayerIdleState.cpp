#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "PlayerIdleState.h"


namespace app
{
    namespace state
    {
        void PlayerIdleState::Enter()
        {
            pPlayer_->SetCurrentIndex(0);

            // 着地したのでフラグをおろす。
            pPlayer_->SetRespawnFlag(false);
        }


        void PlayerIdleState::Update()
        {


            if (pPlayer_->GetKeyDirection().x < 0.0f)
                pPlayer_->SetCurrentIndex(0);

            else
                pPlayer_->SetCurrentIndex(4);

            Vector3& speed = pPlayer_->GetMoveSpeed();
            speed.x = 0.0f;
            speed.z = 0.0f;


            // 座標と描画の同期。    
            pPlayer_->ApplyMovement();

            pPlayer_->pRender_->SetPosition(pPlayer_->GetPlayerPos());
            pPlayer_->pRender_->Update();
        }


        void PlayerIdleState::Exit() {}


        bool PlayerIdleState::RequestID(uint8_t& request)
        {

            // 地面についていたら
            if (pPlayer_->charaCon_.IsOnGround())
            {
                // Aボタンを押した瞬間だけジャンプ開始
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    request = EnPlayerState::enState_Jump;
                    return true;
                }

                auto speedX = g_pad[0]->GetLStickXF() * 1.0f * 10.0f;
                auto speedY = g_pad[0]->GetLStickYF() * -1.0f * 10.0f;
                // x zの移動速度があったらスティックの入力
                if (fabsf(speedX) >= 0.02f || fabsf(speedY) >= 0.02f)
                {
                    request = EnPlayerState::enState_Run;
                    return true;
                }
            }


            // 地面から離れたら落下ステートへ
            else 
            {
                request = EnPlayerState::enState_Fall;
                return true;
            }
            return false;
        }


////        void PlayerIdleState::ApplyMovement()
//        {
//            // 移動処理。
//            const Vector3 pos = pPlayer_->GetCharacterController().Execute(pPlayer_->GetMoveSpeed(), 1.0f / 150.0f);
//            // 座標のセット。
//            pPlayer_->GetCharacterController()->SetPosition(pPlayer_->GetPlayerPos());
//            pPlayer_->pRender_->SetPosition(pPlayer_->GetPlayerPos());
//        }
    }
}

