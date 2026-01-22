#include "stdafx.h"
#include "BossAttackJumpState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto GRAVITY = 0.8f;        // 重力。
    const auto JUMP_POWER = 25.0f;    // ジャンプ力。
    const auto PREPARE_TIME = 1.0f;   // 準備時間。

    const auto CAMERA_EDGE_OFFSET = 25.0f; // カメラ端からのオフセット。
}

namespace app
{
    namespace enemyState
    {
        void BossAttackJumpState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;
            step_ = app::enemyStatus::JumpStep::Prepare;
            timer_ = 0.0f;

            // アニメーションの再生。
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Jump, false, 0.2f);

            // 回転の準備。
            startRot_ = pBoss_->GetRot();
            targetRot_ = startRot_;

            // プレイヤーの方向を向くようにする。
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                Vector3 diff = pPlayer->GetPlayerPos() - pBoss_->GetPos();
                diff.y = 0.0f;

                // 距離が近すぎない場合のみ回転計算。
                if (diff.LengthSq() > 0.1f)
                {
                    targetRot_.SetRotationYFromDirectionXZ(diff);
                }
            }

        }


        void BossAttackJumpState::Update()
        {
            UpdateState();
        }


        void BossAttackJumpState::Exit()
        {
            pBoss_->SettNextInterval(2.5f);
        }


        void BossAttackJumpState::UpdateState()
        {
            switch (step_)
            {
            case app::enemyStatus::JumpStep::Prepare:
                UpdatePrepare();
                break;

            case app::enemyStatus::JumpStep::Jumping:
                UpdateJumping();
                break;

            case app::enemyStatus::JumpStep::Landing:
                UpdateLanding();
                break;

            default:
                break;
            }
        }


        void BossAttackJumpState::UpdatePrepare()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();

            // --- 1. 滑らかに回転 (Slerp) ---
            float rate = timer_ / PREPARE_TIME;
            if (rate > 1.0f)
                rate = 1.0f;

            // 現在の向きからターゲットの向きへ補間
            Quaternion currentRot;
            currentRot.Slerp(rate, startRot_, targetRot_);
            pBoss_->SetRot(currentRot);


            // --- 2. ジャンプ開始判定 ---
            if (timer_ >= PREPARE_TIME)
            {
                step_ = app::enemyStatus::JumpStep::Jumping;

                // スタート地点とゴール地点
                startPos_ = pBoss_->GetPos();

                Vector3 playerPos = startPos_;
                if (auto* pPlayer = pBoss_->GetPlayer())
                    playerPos = pPlayer->GetPlayerPos();


                // 目標座標 (Xはプレイヤー、Zはそのまま=2D軸移動)
                targetPos_ = startPos_;

                if (startPos_.x < playerPos.x)
                    targetPos_.x = playerPos.x - CAMERA_EDGE_OFFSET;

                else
                    targetPos_.x = playerPos.x - CAMERA_EDGE_OFFSET;


                float flightTimeFrame = (2.0f * JUMP_POWER) / GRAVITY;

                // X軸の速度: 距離 / 時間
                // これで「着地する瞬間にちょうどプレイヤーの位置」に到達する
                float distanceX = targetPos_.x - startPos_.x;
                velocity_.x = distanceX / flightTimeFrame;

                // Y軸初速
                velocity_.y = JUMP_POWER;

                // Z軸は移動しない
                velocity_.z = 0.0f;
            }
        }

        void BossAttackJumpState::UpdateJumping()
        {
            // 移動処理
            Vector3 pos = pBoss_->GetPos();

            velocity_.y -= GRAVITY; // 重力落下
            pos += velocity_;

            // 着地判定 (Y <= 0)
            if (pos.y <= 0.0f)
            {
                pos.y = 0.0f;
                pBoss_->SetPos(pos);

                // 着地！
                step_ = app::enemyStatus::JumpStep::Landing;

                // 着地アニメーション再生
                pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Land, false, 0.1f);

            }
            else
            {
                pBoss_->SetPos(pos);
            }
        }

        void BossAttackJumpState::UpdateLanding()
        {
            // 着地アニメーションが終わるのを待つ
            if (!pBoss_->IsPlayingAnimation())
            {
                step_ = app::enemyStatus::JumpStep::Finish;
            }
        }


        bool BossAttackJumpState::IsFinished() const
        {
            return step_ == app::enemyStatus::JumpStep::Finish;
        }
    }
}
