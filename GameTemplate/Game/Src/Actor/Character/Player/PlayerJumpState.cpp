#include "stdafx.h"
#include "PlayerJumpState.h"
#include "Src/Actor/Character/Player.h"

struct PlayerStatus
{
    static constexpr auto GLAVITY = 15.0f; // 重力。


    // 移動速度パラメータ。
    struct Move
    {
        static constexpr float SPEED = 1125.0f;     // 移動速度アップ
        static constexpr float JUMP_POWER = 600.0f; // ジャンプ力アップ
    };

    // ジャンプ用のパラメータ。
    struct Jump
    {
        static constexpr float GLAVITY = 2.5f;         // 落下中の重力倍率。
        static constexpr float CUT = 5.0f;             // ボタンを押した後の重力倍率。。
        static constexpr float FALLINGSPEED = -600.0f; // 落下速度。
    };
};


void PlayerJumpState::Enter()
{
    // ジャンプアニメーション再生。
    pPlayer_->render_.PlayAnimation(EnAnimationClip::animJump);
}


void PlayerJumpState::Update()
{
    // ジャンプと重力の更新処理。
    UpdateJumpAndGravity();

    // 移動処理。
    ApplyMovement();

    pPlayer_->render_.SetPosition(pPlayer_->GetMoveSpeed());
    pPlayer_->render_.Update();
}


void PlayerJumpState::Exit()
{

}


bool PlayerJumpState::RequestID(uint8_t& request)
{
    // 地面についていたら
    if (pPlayer_->GetCharacterController().IsOnGround())
    {
        // ジャンプが終わったらIdel状態へ移行。
        request = EnPlayerState::enState_Idle;
        return true;
    }
    return false;
}


void PlayerJumpState::UpdateJumpAndGravity()
{
    const bool isGround = pPlayer_->GetCharacterController().IsOnGround();

    // --- 地面上での処理 ---
    if (isGround)
    {
        // 地面にいる間は落下速度をリセット
        if (pPlayer_->GetMoveSpeed().y < 0.0f)
            pPlayer_->GetMoveSpeed().y = 0.0f;

        // Aボタンを押した瞬間だけジャンプ開始
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            pPlayer_->GetMoveSpeed().y = pPlayer_->GetMoveSpeed().y = PlayerStatus::Move::JUMP_POWER;
            pPlayer_->SetJumpedThisFrame(true);
        }
    }

    // 地上かつ完全停止中のときは重力をかけない（勝手に動かない）
    if (isGround && pPlayer_->GetMoveSpeed().y <= 0.0f)
        return;

    if (pPlayer_->GetMoveSpeed().y > 0.0f)
    {
        // 上昇中
        // ジャンプボタンを離したらジャンプを早めに切る（低いジャンプ）
        if (!g_pad[0]->IsPress(enButtonA))
            // ジャンプカット時は強めの重力
            pPlayer_->GetMoveSpeed().y -= PlayerStatus::GLAVITY * PlayerStatus::Jump::CUT;

        else
            // 通常上昇中は普通の重力
            pPlayer_->GetMoveSpeed().y -= PlayerStatus::GLAVITY;
    }
    else
        // 下降中：落下中の重力を強める
        pPlayer_->GetMoveSpeed().y -= PlayerStatus::GLAVITY * PlayerStatus::Jump::GLAVITY;

    // 落下速度の制限（下向きがマイナス）
    if (pPlayer_->GetMoveSpeed().y < PlayerStatus::Jump::FALLINGSPEED)
        pPlayer_->GetMoveSpeed().y = PlayerStatus::Jump::FALLINGSPEED;
}


void PlayerJumpState::ApplyMovement()
{
    const Vector3 pos = pPlayer_->GetCharacterController().Execute(pPlayer_->GetMoveSpeed(), 1.0f / 150.0f);

    pPlayer_->SetPlayerPos(pos);
}
