#include "stdafx.h"
#include "PlayerJumpState.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Core/SoundManager.h"

struct PlayerStatus
{
    // 通常の重力。
    static constexpr auto GLAVITY = 8.0f; // 重力。

    // 移動速度パラメータ。
    struct Move
    {
        static constexpr auto JUMP_POWER = 150.0f;  // ジャンプ力アップ
        static constexpr auto WALK_SPEED = 100.0f;  // 地上での移動速度。
        static constexpr auto AIR_MOVE_RATE = 0.7f; // 空中で動ける移動倍率。        
    };

    // ジャンプ用のパラメータ。
    struct Jump
    {
        // ジャンプ中の重力倍率。
        static constexpr auto GLAVITY = 1.2f;
        // ボタンを押した後の重力倍率。。
        static constexpr auto CUT = 2.0f;
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
            pPlayer_->SetCurrentIndex(0); // ジャンプアニメーション

            // ステートに遷移した瞬間にジャンプの初速を与える。
            pPlayer_->GetMoveSpeed().y = PlayerStatus::Move::JUMP_POWER;
            pPlayer_->SetJumpedThisFrame(true);

            // 多段再生防止のため、1回きりの再生とする。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Jump);
        }


        void PlayerJumpState::Update()
        {
            // 1. スティック入力の取得
            Vector3 stick;
            stick.x = g_pad[0]->GetLStickXF();
            stick.y = g_pad[0]->GetLStickYF();

            // 2. カメラの向きに基づいた軸補正（斜め移動防止）
            Vector3 camRight = g_camera3D->GetRight();
            Vector3 camForward = g_camera3D->GetForward();
            camRight.y = 0.0f;
            camForward.y = 0.0f;
            camRight.Normalize();
            camForward.Normalize();

            // 視点による軸の補正（2D/3Dどちらでもステージの軸にスナップさせる）
            if (fabsf(camForward.z) > fabsf(camForward.x))
            {
                // Z軸主体の移動（X軸が横）
                camForward.x = 0.0f;
                camForward.z = (camForward.z > 0.0f) ? 1.0f : -1.0f;
                camRight.z = 0.0f;
                camRight.x = (camRight.x > 0.0f) ? 1.0f : -1.0f;
            }
            else
            {
                // X軸主体の移動（Z軸が横）
                camForward.z = 0.0f;
                camForward.x = (camForward.x > 0.0f) ? 1.0f : -1.0f;
                camRight.x = 0.0f;
                camRight.z = (camRight.z > 0.0f) ? 1.0f : -1.0f;
            }

            // 3. 移動ベクトルの合成
            // 左右移動（stick.x）は常に適用
            Vector3 moveDir = camRight * stick.x;

            // 3Dモードの時のみ、奥行き移動（stick.y）を許可
            if (pPlayer_->GetCameraManager()->GetCurrentCameraMode() != CameraMode::mode2D)
            {
                moveDir += camForward * stick.y;
            }

            // 計算した方向をプレイヤーにセット（Move関数で使用される）
            pPlayer_->SetKeyDirection(moveDir);

            // 4. 移動計算・重力計算
            Move(PlayerStatus::Move::AIR_MOVE_RATE);
            UpdateJumpAndGravity();

            // 5. アニメーションの向き更新（入力の正負で判定）
            if (stick.x < -0.01f)
                pPlayer_->SetCurrentIndex(1); // 左向き
            else if (stick.x > 0.01f)
                pPlayer_->SetCurrentIndex(5); // 右向き

            // 6. 物理の実行（Player.cppの共通処理を呼ぶ）
            pPlayer_->ApplyMovement();

            // 7. 描画位置の同期
            if (pPlayer_->pRender_)
            {
                pPlayer_->pRender_->SetPosition(pPlayer_->GetPlayerPos());
                pPlayer_->pRender_->Update();
            }
        }


        void PlayerJumpState::Exit() {}


        bool PlayerJumpState::RequestID(uint8_t& request)
        {
            // 地面についていたら
            if (pPlayer_->GetCharacterController().IsOnGround() && pPlayer_->GetMoveSpeed().y <= 0.0f)
            {
                // ジャンプが終わったらIdel状態へ移行。
                request = EnPlayerState::enState_Idle;
                return true;
            }

            if (pPlayer_->IsRespawn())
            {
                request = EnPlayerState::enState_Idle;
                return true;
            }

            return false;
        }


        void PlayerJumpState::UpdateJumpAndGravity()
        {
            Vector3& speed = pPlayer_->GetMoveSpeed();
            const bool isGround = pPlayer_->GetCharacterController().IsOnGround();

            // 接地中のリセット
            if (isGround && speed.y <= 0.0f)
            {
                speed.y = 0.0f;
                return;
            }

            if (speed.y > 0.0f)
            {
                // 上昇中
                if (!g_pad[0]->IsPress(enButtonA))
                {
                    speed.y -= PlayerStatus::GLAVITY * PlayerStatus::Jump::CUT;
                }
                else
                    speed.y -= PlayerStatus::GLAVITY;
            }
            else
            {
                // 下降中
                speed.y -= PlayerStatus::GLAVITY * PlayerStatus::Jump::GLAVITY;
            }

            if (speed.y < PlayerStatus::Jump::FALLINGSPEED)
                speed.y = PlayerStatus::Jump::FALLINGSPEED;
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

