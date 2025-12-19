#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "PlayerIdleState.h"

void PlayerIdleState::Enter()
{
    pPlayer_->render_.PlayAnimation(EnAnimationClip::animIdle);
}


void PlayerIdleState::Update()
{
    // 待機中はPlayerの移動速度を0にする。
    Vector3& speed = pPlayer_->GetMoveSpeed();
    speed.x = 0.0f;
    speed.z = 0.0f;

    ApplyMovement();

    pPlayer_->render_.SetPosition(pPlayer_->GetMoveSpeed());
    pPlayer_->render_.Update();
}


void PlayerIdleState::Exit()
{

}



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

        auto speedX = g_pad[0]->GetLStickXF() *  1.0f * 10.0f;
        auto speedY = g_pad[0]->GetLStickYF() * -1.0f * 10.0f;
        // x zの移動速度があったらスティックの入力
        if (fabsf(speedX) >= 0.02f || fabsf(speedY) >= 0.02f)
        {
            request = EnPlayerState::enState_Run;
            return true;
        }
    }
    return false;
}


void PlayerIdleState::ApplyMovement()
{
    // 移動処理。

    const Vector3 pos =  pPlayer_->GetCharacterController().Execute(pPlayer_->GetMoveSpeed(), 1.0f / 150.0f);
    // 座標のセット。
    pPlayer_->GetCharacterController().SetPosition(pPlayer_->GetPlayerPos());
    pPlayer_->GetModelRender().SetPosition(pPlayer_->GetPlayerPos());
}

