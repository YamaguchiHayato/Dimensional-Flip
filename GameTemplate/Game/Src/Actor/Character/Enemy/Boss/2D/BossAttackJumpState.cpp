#include "stdafx.h"
#include "BossAttackJumpState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Core/SoundManager.h"
#include "Src/Presentation/UI/BossAttackHudHelper.h"

namespace
{
    const auto GRAVITY = 0.4f;             //! 重力。
    const auto JUMP_POWER = 12.0f;         //! ジャンプの初速。
    const auto PREPARE_TIME = 1.0f;        //! ジャンプの準備時間。
    const auto CAMERA_EDGE_OFFSET = 15.0f; //! カメラ端のオフセット値。
    const auto STAGE_LIMIT_X = 35.0f;      //! ステージのX方向の制限値。
    const int MAX_JUMP_COUNT = 3;          //! 最大ジャンプ回数。
    const auto LEG_HIT_RADIUS = 15.0f;     //! ボスの足元の当たり判定の半径。
} 

namespace app
{
    namespace enemyState
    {
        void BossAttackJumpState::Enter(app::enemy::Boss* pBoss)
        {
            /* 初期化。*/
            pBoss_ = pBoss;
            step_ = app::enemyStatus::JumpStep::Prepare;
            timer_ = 0.0f;
            jumopCount_ = 0;
            hasHitPlayer_ = false;

            /* アニメーションを再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Idle, true, 0.2f);

            /* プレイヤーの位置に応じて、ボスの向きを変える。*/ 
            faceAngle_.SetRotationDegY(180.0f);
            pBoss_->SetRot(faceAngle_);

            /* 攻撃タイプをジャンプ攻撃に設定。*/
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Jump);

            /* 攻撃タイプのセット。*/
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Jump);
        }


        void BossAttackJumpState::Update()
        {
            /* 状態の更新。*/
            UpdateState();
        }


        void BossAttackJumpState::Exit()
        {
            /* ジャンプ攻撃終了時の処理。*/
            pBoss_->SettNextInterval(2.5f);

            /* 攻撃タイプのリセット。 */
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Jump);

        }


        void BossAttackJumpState::UpdateState()
        {
            /* ジャンプ攻撃の状態に応じて処理を分岐。*/
            switch (step_)
            {
            case enemyStatus::JumpStep::Prepare:
                UpdatePrepare();
                break;

            case enemyStatus::JumpStep::Jumping:
                UpdateJumping();
                break;

            case enemyStatus::JumpStep::Landing:
                UpdateLanding();
                break;

            default:
                break;
            }
        }


        void BossAttackJumpState::UpdatePrepare()
        {
            /* タイマーの取得。*/
            timer_ += g_gameTime->GetFrameDeltaTime();

            /* ジャンプの準備時間が経過したら、ジャンプを開始する。*/
            if (timer_ >= PREPARE_TIME)
            {
                /* ジャンプSEの再生。*/
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_BossJump, 2.0f);

                /* ジャンプ状態に遷移。*/
                step_ = app::enemyStatus::JumpStep::Jumping;

                /* ジャンプの開始位置を取得。*/
                startPos_ = pBoss_->GetPos();

                /* プレイヤーの位置を取得。*/
                Vector3 playerPos = startPos_;
                if (auto* pPlayer = pBoss_->GetPlayer())
                    playerPos = pPlayer->GetPlayerPos();

                /* ジャンプの目標位置を設定。*/
                targetPos_.x = playerPos.x;
                targetPos_.y = 0.0f; // 地面に着地
                targetPos_.z = 0.0f;

                /* ステージの制限範囲内に収める。*/
                if (targetPos_.x > STAGE_LIMIT_X)
                    targetPos_.x = STAGE_LIMIT_X;
                if (targetPos_.x < -STAGE_LIMIT_X)
                    targetPos_.x = -STAGE_LIMIT_X;
                
                /* ジャンプの飛行時間を計算。*/
                float flightTimeFrame = (2.0f * JUMP_POWER) / GRAVITY;

                /* 水平方向の距離を計算。*/
                float distanceX = targetPos_.x - startPos_.x;

                /* 水平方向の速度を計算。*/
                velocity_.x = distanceX / flightTimeFrame;

                /* 垂直方向の速度を設定。*/
                velocity_.y = JUMP_POWER;
                velocity_.z = 0.0f;

                /* ボスの向きをプレイヤーの方向に向ける。*/
                Quaternion targetRot;
                if (distanceX > 0)
                    targetRot.SetRotationDegY(-90.0f);
                else
                    targetRot.SetRotationDegY(90.0f);

                /* ボスの回転を設定。*/
                pBoss_->SetRot(targetRot);

                /* ジャンプアニメーションを再生。*/
                pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Jump, false, 0.1f);
            }
        }


        void BossAttackJumpState::UpdateJumping()
        {
            /* ボスの現在位置を取得。*/
            Vector3 pos = pBoss_->GetPos();

            /* 重力を適用して、ボスの位置を更新。*/
            velocity_.y -= GRAVITY; // 重力落下
            pos += velocity_;

            /* ボスが地面に着地したかを判定。*/
            if (pos.y <= 0.0f)
            {
                /* ボスの位置を地面に固定。*/
                pos.y = 0.0f;
                pBoss_->SetPos(pos);

                /* 着地時のエフェクトを初期化。*/
                InitEffect();

                /* 着地SEの再生。*/
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Land, 2.0f);

                /* プレイヤーへのヒット判定フラグをリセット。*/
                hasHitPlayer_ = false;

                /* 着地状態に遷移。*/
                step_ = app::enemyStatus::JumpStep::Landing;

                /* 着地アニメーションを再生。*/
                pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Land, false, 0.1f);

            }
            else
                pBoss_->SetPos(pos);
        }


        void BossAttackJumpState::UpdateLanding()
        {
            /* プレイヤーへのヒット判定を行う。*/
            if (!hasHitPlayer_)
            {
                /* プレイヤーとの衝突判定を行い、ヒットした場合はフラグを立てる。*/
                if (CheckPlayerCollision())
                    hasHitPlayer_ = true;
            }


            /* 着地アニメーションが終了したかを判定。*/
            if (!pBoss_->IsPlayingAnimation())
            {
                /* ジャンプ回数を加算。*/
                AddJumpCount();

                /* 最大ジャンプ回数に達していない場合は、再度ジャンプの準備状態に遷移。*/
                if (jumopCount_ < MAX_JUMP_COUNT)
                {
                    /* ジャンプの準備状態に遷移。*/
                    step_ = app::enemyStatus::JumpStep::Prepare;
                    /* タイマーをリセット。*/
                    timer_ = 0.0f;
                    /* プレイヤーへのヒット判定フラグをリセット。*/
                    hasHitPlayer_ = false;

                    /* 待機アニメーションを再生。*/
                    pBoss_->LoadAnimation(app::enemyStatus::bossAnim_Idle, true, 0.1f);
                }

                else
                    step_ = app::enemyStatus::JumpStep::Finish;
            }
        }


        void BossAttackJumpState::InitEffect()
        {
            /* 着地時の衝撃波エフェクトを生成。*/
            auto* pShockWaveEffect = NewGO<EffectEmitter>(0);
            pShockWaveEffect->Init(app::enemyStatus::EffectID::effect_ShockWave);
            pShockWaveEffect->SetPosition(pBoss_->GetPos());
            pShockWaveEffect->SetScale(Vector3::One);
            pShockWaveEffect->Play();
        }


        bool BossAttackJumpState::IsFinished() const
        {
            /* ジャンプ攻撃が終了したかを判定。*/
            return step_ == app::enemyStatus::JumpStep::Finish;
        }


        bool BossAttackJumpState::CheckPlayerCollision()
        {
            /* プレイヤーの存在を確認。*/
            auto* pPlayer = pBoss_->GetPlayer();
            if (!pPlayer)
                return false;

            /* プレイヤーの座標を取得。*/
            Vector3 playerPos = pPlayer->GetPlayerPos();

            /* ボスの足元の座標を取得。*/
            Vector3 bossLegPos = pBoss_->GetPos();

            /* プレイヤーとボスの足元の距離を計算。*/
            Vector3 diff = playerPos - bossLegPos;
            diff.y = 0.0f; 

            /* プレイヤーがボスの足元の範囲内にいるかを判定。*/
            if (diff.LengthSq() <= LEG_HIT_RADIUS * LEG_HIT_RADIUS)
            {
                /* プレイヤーにダメージを与える。*/
                pPlayer->OnDamage(1);
                return true;
            }
            return false;
        }
    }
}
