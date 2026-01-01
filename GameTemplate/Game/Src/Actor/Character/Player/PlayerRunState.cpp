#include "stdafx.h"

#include "PlayerRunState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"


namespace
{
    const auto MAX_TIME = 0.8f;

    struct Move
    {
        static constexpr float SPEED = 100.0f; // 移動速度アップ
    };
}

namespace app
{
    namespace state
    {
        void PlayerRunState::Enter()
        {
            // 走りアニメーション再生
            pPlayer_->SetCurrentIndex(0); 
        }


        void PlayerRunState::Update()
        {
            // 地面にいるかチェック
            bool isGround = pPlayer_->GetCharacterController().IsOnGround();

            if (isGround)
                pPlayer_->SetCanAirControl(true);


            UpdatePlayerMove();

            // 移動計算。
            CalculateRunMovement();


            pPlayer_->ApplyMovement();

            pPlayer_->pRender_->SetPosition(pPlayer_->GetPlayerPos());
            pPlayer_->pRender_->Update();
        }


        void PlayerRunState::Exit() {}


        bool PlayerRunState::RequestID(uint8_t& request)
        {
            // 1. まずジャンプをチェック
            if (g_pad[0]->IsTrigger(enButtonA))
            {
                request = EnPlayerState::enState_Jump;
                return true;
            }


            // 2. 次に「地面から離れたか」をチェック（タスク：OnGroundがFalseの時に落下へ）
            if (!pPlayer_->charaCon_.IsOnGround())
            {
                request = EnPlayerState::enState_Fall;
                return true;
            }


            // 3. 入力がなくなったら待機へ
            Vector3 stick = {g_pad[0]->GetLStickXF(), 0.0f, g_pad[0]->GetLStickYF()};
            if (fabsf(stick.x) < 0.20f && fabsf(stick.z) < 0.20f)
            {
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


        void PlayerRunState::CalculateRunMovement()
        {
            // カメラマネージャーの取得
            CameraManager* pCameraManager = pPlayer_->GetCameraManager();
            if (!pCameraManager)
                return;

            // スティック入力取得
            Vector3 stickL;
            stickL.x = g_pad[0]->GetLStickXF();
            stickL.y = g_pad[0]->GetLStickYF();

            // デッドゾーン
            if (fabsf(stickL.x) < 0.2f)
                stickL.x = 0.0f;
            if (fabsf(stickL.y) < 0.2f)
                stickL.y = 0.0f;


            // キー入力保持。
            pPlayer_->SetKeyDirection(stickL);


            // Y軸の速度（重力など）は維持しつつ、XZ速度をリセットして再計算
            Vector3& moveSpeed = pPlayer_->GetMoveSpeed();
            float currentY = moveSpeed.y;
            moveSpeed.x = 0.0f;
            moveSpeed.z = 0.0f;


            // カメラ情報の取得
            Vector3 camRight = g_camera3D->GetRight();
            Vector3 camForward = g_camera3D->GetForward();
            camRight.y = 0.0f;
            camForward.y = 0.0f;
            camRight.Normalize();
            camForward.Normalize();


            // 視点による軸の補正（2.5D的な挙動のための補正）
            if (fabsf(camForward.z) > fabsf(camForward.x))
            {
                // Z軸主体の移動
                camForward.x = 0.0f;
                camForward.z = (camForward.z > 0.0f) ? 1.0f : -1.0f;
                camRight.z = 0.0f;
                camRight.x = (camRight.x > 0.0f) ? 1.0f : -1.0f;
            }
            else
            {
                // X軸主体の移動
                camForward.z = 0.0f;
                camForward.x = (camForward.x > 0.0f) ? 1.0f : -1.0f;
                camRight.x = 0.0f;
                camRight.z = (camRight.z > 0.0f) ? 1.0f : -1.0f;
            }

            bool isRotatedView = fabsf(camRight.z) > fabsf(camRight.x);
            CameraMode currentMode = pCameraManager->GetCurrentCameraMode();

            auto walkSpeed = pPlayer_->GetWalkSpeed();
            moveSpeed.y = currentY;

            Vector3 targetVelocity = (camRight * stickL.x) + (camForward * stickL.y);

            // Y軸（重力）は保存しておいたものを戻す
            moveSpeed.x = targetVelocity.x * walkSpeed;
            moveSpeed.z = targetVelocity.z * walkSpeed;
            moveSpeed.y = currentY;
        }


        void PlayerRunState::ApplyMovement()
        {
            Vector3 newPos = pPlayer_->GetCharacterController().Execute(pPlayer_->GetMoveSpeed(), 1.0f / 150.0f);

            // 移動処理。
            // 座標のセット。
            pPlayer_->SetPlayerPos(newPos);
            pPlayer_->GetCharacterController().SetPosition(newPos);
        }


        void PlayerRunState::UpdatePlayerMove()
        {
            currentTime_ += g_gameTime->GetFrameDeltaTime();

            if (currentTime_ >= 0.4f)
                currentTime_ = 0.0f;

            if (pPlayer_->GetKeyDirection().x < 0.0f)
            {
                if (currentTime_ < 0.1f)
                    pPlayer_->SetCurrentIndex(2);

                else if (currentTime_ < 0.2f)
                    pPlayer_->SetCurrentIndex(0);

                else if (currentTime_ < 0.3f)
                    pPlayer_->SetCurrentIndex(3);

                else if (currentTime_ < 0.4f)
                    pPlayer_->SetCurrentIndex(0);
            }

            else
            {
                if (currentTime_ < 0.1f)
                    pPlayer_->SetCurrentIndex(6);

                else if (currentTime_ < 0.2f)
                    pPlayer_->SetCurrentIndex(4);

                else if (currentTime_ < 0.3f)
                    pPlayer_->SetCurrentIndex(7);

                else
                    pPlayer_->SetCurrentIndex(4);
            }



        }
    }
}

