#include "stdafx.h"
#include "BossAttackJumpState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto GRAVITY = 0.4f;        // 重力。
    const auto JUMP_POWER = 12.0f;    // ジャンプ力。

    const auto PREPARE_TIME = 1.0f;   // 準備時間。

    const auto CAMERA_EDGE_OFFSET = 15.0f; // カメラ端からのオフセット。

    const auto STAGE_LIMIT_X = 35.0f; // ステージ端のX座標。

    const int MAX_JUMP_COUNT = 3;

    const auto LEG_HIT_RADIUS = 5.0f; // 足元の当たり判定の半径。
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
            jumopCount_ = 0;

            // アニメーションの再生。
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Jump, false, 0.2f);

            // 正面を向くようにする。
            faceAngle_.SetRotationDegY(180.0f);
            pBoss_->SetRot(faceAngle_);
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

            // --- ジャンプ開始判定 ---
            if (timer_ >= PREPARE_TIME)
            {
                // ジャンプ開始
                step_ = app::enemyStatus::JumpStep::Jumping;

                // スタート地点を取得
                startPos_ = pBoss_->GetPos();

                // プレイヤーの現在地を取得
                Vector3 playerPos = startPos_;
                if (auto* pPlayer = pBoss_->GetPlayer())
                    playerPos = pPlayer->GetPlayerPos();

                // --- 目標座標の設定 ---
                targetPos_.x = playerPos.x;
                targetPos_.y = 0.0f; // 地面に着地
                targetPos_.z = 0.0f;

                // 目標座標 (Xはプレイヤー、Zはそのまま=2D軸移動)
                targetPos_ = startPos_;

                if (startPos_.x < playerPos.x)
                    targetPos_.x = playerPos.x - CAMERA_EDGE_OFFSET;

                else
                    targetPos_.x = playerPos.x + CAMERA_EDGE_OFFSET;


                float flightTimeFrame = (2.0f * JUMP_POWER) / GRAVITY;

                // 必要な水平距離の計算
                float distanceX = targetPos_.x - startPos_.x;

                // 水平速度の決定 (速度 = 距離 / 時間)
                velocity_.x = distanceX / flightTimeFrame;

                // ジャンプの初速を設定
                velocity_.y = JUMP_POWER;
                velocity_.z = 0.0f;

                // ジャンプアニメーション再生
                pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Jump, false, 0.1f);
            }
        }


        void BossAttackJumpState::UpdateJumping()
        {
            // 移動処理
            Vector3 pos = pBoss_->GetPos();

            velocity_.y -= GRAVITY; // 重力落下
            pos += velocity_;

            // 空中での足元の当たり判定。
            CheckPlayerCollision();

            // 着地判定 (Y <= 0)
            if (pos.y <= 0.0f)
            {
                pos.y = 0.0f;
                pBoss_->SetPos(pos);

                // 衝撃波エフェクトを再生。
                InitEffect();

                // 着地
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
                // ジャンプの回数をカウント。
                AddJumpCount();

                // 最大ジャンプ回数に達していなければ、再度ジャンプへ。
                if (jumopCount_ < MAX_JUMP_COUNT)
                {
                    step_ = app::enemyStatus::JumpStep::Prepare;
                    timer_ = 0.0f;
                }

                else
                {
                    step_ = app::enemyStatus::JumpStep::Finish;
                }

            }
        }


        void BossAttackJumpState::InitEffect()
        {
            auto* pShockWaveEffect = NewGO<EffectEmitter>(0);
            pShockWaveEffect->Init(app::enemyStatus::EffectID::effect_ShockWave);
            pShockWaveEffect->SetPosition(pBoss_->GetPos());
            pShockWaveEffect->SetScale(Vector3::One);
            pShockWaveEffect->Play();
        }


        void BossAttackJumpState::InitEffect()
        {
            auto* pShockWaveEffect = NewGO<EffectEmitter>(0);
            pShockWaveEffect->Init(app::enemyStatus::EffectID::effect_ShockWave);
            pShockWaveEffect->SetPosition(pBoss_->GetPos());
            pShockWaveEffect->SetScale(Vector3::One);
            pShockWaveEffect->Play();
        }


        bool BossAttackJumpState::IsFinished() const
        {
            return step_ == app::enemyStatus::JumpStep::Finish;
        }


        bool BossAttackJumpState::CheckPlayerCollision()
        {
            auto* pPlayer = pBoss_->GetPlayer();
            if (!pPlayer)
                return false;

            // プレイヤーの判定用の座標。
            Vector3 playerPos = pPlayer->GetPlayerPos();
            playerPos.y += 10.0f;

            // ボスの現在の足元の座標を取得。
            Vector3 bossLegPos = pBoss_->GetPos();

            // 距離の計算。
            Vector3 diff = playerPos - bossLegPos;

            if (diff.LengthSq() <= LEG_HIT_RADIUS * LEG_HIT_RADIUS)
            {
                // プレイヤーにダメージを与える。
                pPlayer->OnDamage(1);
                return true;
            }
            return false;
        }
    }
}
