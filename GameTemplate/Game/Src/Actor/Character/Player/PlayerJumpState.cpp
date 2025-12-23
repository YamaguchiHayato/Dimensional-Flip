#include "stdafx.h"
#include "PlayerJumpState.h"
#include "Src/Actor/Character/Player.h"

struct PlayerStatus
{
    // 通常の重力。
    static constexpr auto GLAVITY = 15.0f; // 重力。


    // 移動速度パラメータ。
    struct Move
    {
        static constexpr float JUMP_POWER = 150.0f;  // ジャンプ力アップ
        static constexpr float WALK_SPEED = 100.0f;  // 地上での移動速度。
        static constexpr float AIR_MOVE_RATE = 0.6f; // 空中で動ける移動倍率。        
    };

    // ジャンプ用のパラメータ。
    struct Jump
    {
        // ジャンプ中の重力倍率。
        static constexpr float GLAVITY = 2.5f;
        // ボタンを押した後の重力倍率。。
        static constexpr float CUT = 5.0f;
        // 落下速度。
        static constexpr float FALLINGSPEED = -200.0f; 
    };
};


namespace app
{
    namespace state
    {
        void PlayerJumpState::Enter()
        {
            // ジャンプアニメーション再生。
            pPlayer_->render_.PlayAnimation(EnAnimationClip::animJump);
        }


        void PlayerJumpState::Update()
        {
            if (pPlayer_->GetKeyDirection().x < 0.0f)
                pPlayer_->SetCurrentIndex(1);

            else
                pPlayer_->SetCurrentIndex(5);

            //Vector3 keyDir = pPlayer_->GetKeyDirection();
            //if (keyDir.x < 0.0f)
            //{
            //    pPlayer_->SetCurrentIndex(1); // 左向き
            //}
            //else if (keyDir.x > 0.0f)
            //{
            //    pPlayer_->SetCurrentIndex(5); // 右向き
            //}

            //// 空中での移動処理。
            //Move(PlayerStatus::Move::AIR_MOVE_RATE);

            // ジャンプと重力の更新処理。
            UpdateJumpAndGravity();

            // 移動処理。
            ApplyMovement();

            pPlayer_->render_.SetPosition(pPlayer_->GetMoveSpeed());
            pPlayer_->render_.Update();
        }


        void PlayerJumpState::Exit() {}


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


        void PlayerJumpState::Move(float speedRate)
        {
            // プレイヤーの入力方向を取得。
            const Vector3& keyDir = pPlayer_->GetKeyDirection();

            pPlayer_->GetMoveSpeed().x = keyDir.x * PlayerStatus::Move::WALK_SPEED * speedRate;
            pPlayer_->GetMoveSpeed().z = keyDir.z * PlayerStatus::Move::WALK_SPEED * speedRate;
        }
    }
}

