#include "stdafx.h"
#include "BossAttackJumpState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Core/SoundManager.h"

namespace
{
    // 重力。
    const auto GRAVITY = 0.4f;        

    // ジャンプ力。
    const auto JUMP_POWER = 12.0f;    

    // 準備時間。
    const auto PREPARE_TIME = 1.0f;   

    // カメラ端からのオフセット。
    const auto CAMERA_EDGE_OFFSET = 15.0f; 

    // ステージ端のX座標。
    const auto STAGE_LIMIT_X = 35.0f; 

    // ジャンプの最大回数。
    const int MAX_JUMP_COUNT = 3;

    // 足元の当たり判定の半径。
    const auto LEG_HIT_RADIUS = 15.0f; 
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

            // ヒットフラグを初期化。
            hasHitPlayer_ = false;

            // アニメーションの再生。
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Idle, true, 0.2f);

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
                // ジャンプ(上昇)SEの再生。
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_BossJump, 2.0f);

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

                // ステージ外に行かないように制限。
                if (targetPos_.x > STAGE_LIMIT_X)
                    targetPos_.x = STAGE_LIMIT_X;
                if (targetPos_.x < -STAGE_LIMIT_X)
                    targetPos_.x = -STAGE_LIMIT_X;

                // 滞空時間の計算 
                float flightTimeFrame = (2.0f * JUMP_POWER) / GRAVITY;

                // 必要な水平距離の計算
                float distanceX = targetPos_.x - startPos_.x;

                // 水平速度の決定 (速度 = 距離 / 時間)
                velocity_.x = distanceX / flightTimeFrame;

                // ジャンプの初速を設定
                velocity_.y = JUMP_POWER;
                velocity_.z = 0.0f;

                Quaternion targetRot;
                if (distanceX > 0)
                    targetRot.SetRotationDegY(-90.0f);

                else
                    targetRot.SetRotationDegY(90.0f);
                pBoss_->SetRot(targetRot);

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

            // 着地判定 (Y <= 0)
            if (pos.y <= 0.0f)
            {
                pos.y = 0.0f;
                pBoss_->SetPos(pos);

                // 衝撃波エフェクトを再生。
                InitEffect();

                // 着地SEの再生。
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Land, 2.0f);

                // 着地時にヒットフラグをリセットする。
                hasHitPlayer_ = false;

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
            // まだ、ヒットしていなければ、当たり判定を行う。
            if (!hasHitPlayer_)
            {
                if (CheckPlayerCollision())
                    hasHitPlayer_ = true;
            }


            // 着地アニメーションが終わるのを待つ
            if (!pBoss_->IsPlayingAnimation())
            {
                // ジャンプの回数をカウント。
                AddJumpCount();

                // 最大ジャンプ回数に達していなければ、再度ジャンプへ。
                if (jumopCount_ < MAX_JUMP_COUNT)
                {
                    // ジャンプの準備へ。
                    step_ = app::enemyStatus::JumpStep::Prepare;
                    // タイマーをリセット。
                    timer_ = 0.0f;
                    // ヒットフラグをリセット。
                    hasHitPlayer_ = false;

                    // 次のジャンプまでの待機中、Idleアニメーションを再生する。
                    pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Idle, true, 0.1f);
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

            // ボスの現在の足元の座標を取得。
            Vector3 bossLegPos = pBoss_->GetPos();

            // 距離の計算。
            Vector3 diff = playerPos - bossLegPos;
            diff.y = 0.0f; 

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
