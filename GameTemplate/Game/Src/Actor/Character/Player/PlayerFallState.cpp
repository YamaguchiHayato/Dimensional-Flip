#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Character/Player/PlayerFallState.h"


void PlayerFallState::Enter()
{
    //　めも。
    // カメラを切り替る角度を正確に調査。

    // 落下処理をどこに入れんねん。
    // → 多分ジャンプすテートに。
















}

void PlayerFallState::Update()
{
    // 


    //pPlayer_->GetMoveSpeed():

    //pos_ = respwanPos_;
    //rot_ = respwanRot_;

    //pPlayer_->render_.SetPosition(pos_);
    //pPlayer_->render_.SetRotation(rot_);
    //pPlayer_->GetCharacterController().SetPosition(pos_);

    //// 移動速度のリセット。
    //moveSpeed_ = Vector3::Zero;

}


void PlayerFallState::Exit()
{

}

bool PlayerFallState::RequestID(uint8_t & request)
{
    
    return false;
}
