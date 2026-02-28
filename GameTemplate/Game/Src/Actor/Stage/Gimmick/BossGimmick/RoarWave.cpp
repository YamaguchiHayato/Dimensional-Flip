#include "stdafx.h"

#include "RoarWave.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"

namespace
{
    const auto WAVE_SPEED = 60.0f;
    const auto HIT_RADIUS = 4.0f;

    const auto START_SCALE = 2.0f;
    const auto GROWTH_SPEED = 3.0f;

    const Vector3 SCALE_2D = Vector3(1.5f, 0.5f, 1.5f);

    // 削除座標制限
    const auto DELETE_LIMIT_Z = 450.0f;
    const auto DELETE_LIMIT_X = 450.0f;
} // namespace

namespace app
{
    namespace gimmick
    {
        RoarWave::~RoarWave()
        {
            pThunderEffect_ = nullptr;
        }


        bool RoarWave::Start()
        {
            pPlayer_ = FindGO<Player>("player");

            currentScale_ = Vector3::One * START_SCALE;

            // エフェクトの生成。
            pThunderEffect_ = NewGO<EffectEmitter>(0);

            if (pThunderEffect_)
            {
                pThunderEffect_->Init(app::enemyStatus::EffectID::effect_thunder);
                pThunderEffect_->SetScale(currentScale_);
                pThunderEffect_->Play();
                pThunderEffect_->SetPosition(position_);
            }

            return true;
        }


        void RoarWave::Update()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();

            // --- 移動処理 ---
            position_ += direction_ * speed_ * deltaTime;

            // --- 巨大化計算 (3D用) ---
            // 2Dの時も裏で計算だけは進めておく
            auto growth = GROWTH_SPEED * deltaTime;
            currentScale_ += Vector3(growth, growth, 0.0f);

            // --- カメラモード判定と表示サイズ決定 ---
            Vector3 displayScale = currentScale_; // 基本は巨大化サイズ
            if (pPlayer_)
            {
                if (auto* pCamMan = pPlayer_->GetCameraManager())
                {
                    // 2Dモードの時は、小さく固定する
                    if (pCamMan->GetCurrentCameraMode() == CameraMode::mode2D)
                    {
                        displayScale = SCALE_2D;
                    }
                }
            }

            // --- エフェクト更新 ---
            if (pThunderEffect_)
            {
                if (!pThunderEffect_->IsDead())
                {
                    pThunderEffect_->SetPosition(position_);
                    pThunderEffect_->SetScale(displayScale); // ★決定したサイズを適用
                }
                else
                    CreatethunderEffect();
            }

            else
                CreatethunderEffect();

            // --- 当たり判定 ---
            if (CreateCollision())
                return;

            // --- 画面外削除判定 ---
            isOutOfRangeZ_ = (position_.z > DELETE_LIMIT_Z) || (position_.z < -DELETE_LIMIT_Z);
            isOutOfRangeX_ = (position_.x > DELETE_LIMIT_X) || (position_.x < -DELETE_LIMIT_X);


            if (isOutOfRangeZ_ || isOutOfRangeX_)
            {
                if (pThunderEffect_ && !pThunderEffect_->IsDead())
                    DeleteGO(pThunderEffect_);

                pThunderEffect_ = nullptr;
                DeleteGO(this);
            }
        }


        bool RoarWave::CreateCollision()
        {
            if (!pPlayer_)
                return false;

            // プレイヤーとの距離を計算。
            playerPos_ = pPlayer_->GetPlayerPos();
            diff_ = position_ - playerPos_;

            // カメラモードと管理クラスを取得。
            auto* pCameraManager = pPlayer_->GetCameraManager();
            auto getCameraMode = pCameraManager->GetCurrentCameraMode() == CameraMode::mode2D;
            if (pCameraManager)
            {
                if (getCameraMode)
                    currentScale_ = SCALE_2D;
            }

            // 当たり判定の半径を、現在のスケールに応じて拡大。
            hitHeight_ = HIT_RADIUS * currentScale_.y;
            hitRadius_ = HIT_RADIUS * currentScale_.x;

            // Playerが指定した高さ以下かどうかを検知させる。
            heightCheck_ = (playerPosition_.y < hitHeight_);

            if (heightCheck_ && diff_.LengthSq() <= hitRadius_ * hitRadius_)
            {
                // ダメージ処理。
                pPlayer_->OnDamage(1);

                // エフェクトの削除。
                if (pThunderEffect_ && !pThunderEffect_->IsDead())
                {
                    // エフェクトの削除。
                    DeleteGO(pThunderEffect_);
                }
                // ポインタをリセット。
                pThunderEffect_ = nullptr;
                DeleteGO(this);

                return true;
            }
            return false;
        }

        void RoarWave::CreatethunderEffect()
        {
            pThunderEffect_ = NewGO<EffectEmitter>(0);
            pThunderEffect_->Init(app::enemyStatus::EffectID::effect_thunder);
            pThunderEffect_->SetPosition(position_);
            pThunderEffect_->SetScale(currentScale_);
            pThunderEffect_->Play();
        }
    } 
} 
