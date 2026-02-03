#include "stdafx.h"

#include "FireBall.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Player/Player.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
    const auto LIFE_TIME = 8.0f;
    const auto HIT_RADIUS = 3.0f;
}


namespace app
{
    namespace gimmick
    {
        FireBall::~FireBall()
        {
            if (pEffectEmitter_)
                pEffectEmitter_ = nullptr;
        }


        bool FireBall::Start()
        {
            // Playerを探索。
            pPlayer_ = FindGO<Player>("player");

            // エフェクトを生成。
            CreateEffect();

            // エフェクトの座標を初期化。
            UpdateEffectPosition();

            return true;
        }


        void FireBall::Update()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();

            // 移動
            position_ += direction_ * speed_ * deltaTime;

            if (pEffectEmitter_ && pEffectEmitter_->IsPlay())
                UpdateEffectPosition();

            else
            {
                DeleteGO(this);
                return;
            }


            if (CheckPlayerCollision())
                return;

            lifeTime_ += deltaTime;

            // 寿命 (speed_ > 0 の弾幕用)
            if (speed_ > 0.0f && lifeTime_ >= LIFE_TIME)
            {
                DeleteGO(this);
                return;
            }
        }


        void FireBall::SetParameter(const Vector3& startPos, Player* target, float speed)
        {
            position_ = startPos;
            speed_ = speed;

            if (target)
            {
                targetPos_ = target->GetPlayerPos();
                targetPos_.y += 20.0f; // プレイヤーの少し上を狙う

                direction_ = targetPos_ - startPos;

                // 正規化
                if (direction_.LengthSq() > 0.001f)
                {
                    direction_.Normalize();
                }
                else
                {
                    // 万が一重なっていた場合などは右へ
                    direction_ = Vector3::Right;
                }
            }
        }


        void FireBall::SetDirection(const Vector3& spawnPos, const Vector3& direction, float progresSpeed)
        {
            // 生成位置をセット(画面外)。
            SetPosition(spawnPos);

            // 進行方向をセット(平行)。
            direction_ = direction;

            // 進行方向ベクトルの正規化。
            if (direction_.LengthSq() > 0.001f)
                direction_.Normalize();

            // 速度をセット。
            SetSpeed(progresSpeed);
        }


        void FireBall::UpdateEffectPosition()
        {
            if (pEffectEmitter_)
            {
                pEffectEmitter_->SetPosition(position_);
                pEffectEmitter_->SetScale(scale_);

                // 移動方向がある場合、その方向を向かせる
                if (direction_.LengthSq() > 0.001f)
                {
                    // 方向ベクトルから回転を作成 (Z軸を進行方向に向ける)
                    Quaternion lookRot;
                    lookRot.SetRotation(Vector3::Front, direction_);

                    // 外部指定の回転(rotation_)がある場合はそちらを優先、
                    // なければ移動方向(lookRot)を使用
                    // ここでは弾幕(speed > 0)は進行方向、ブレス(speed == 0)は外部指定を使用する想定
                    if (speed_ > 0.0f)
                        rotation_ = lookRot;

                }

                // 最終的な回転 = ベースの回転 * オフセット
                // オフセットで「垂直なエフェクト」を「横向き」に倒します
                Quaternion finalRot = rotationOffset_ * rotation_;
                pEffectEmitter_->SetRotation(finalRot);
            }
        }


        bool FireBall::CheckPlayerCollision()
        {
            if (!pPlayer_ || !isCollisionEnabled_)
                return false;

            Vector3 playerPos = pPlayer_->GetPlayerPos();

            Vector3 diff = playerPos - position_;

            // 当たり判定を計算。
            auto fireBallRadius = HIT_RADIUS * scale_.x;

            // 最終的な当たり判定を乗算。
            auto collisionRadius = fireBallRadius * fireBallRadius;

            if (diff.LengthSq() <= collisionRadius)
            {
                pPlayer_->OnDamage(2);
                DeleteGO(this);
                return true;
            }


            return false;
        }


        void FireBall::CreateEffect()
        {
            pEffectEmitter_ = NewGO<EffectEmitter>(0);

            if (pEffectEmitter_)
            {
                pEffectEmitter_->Init(app::enemyStatus::EffectID::effect_FireBall);
                pEffectEmitter_->SetPosition(position_);
                pEffectEmitter_->SetScale(scale_);
                pEffectEmitter_->Play();
            }
        }
    } // namespace gimmick
} // namespace app
