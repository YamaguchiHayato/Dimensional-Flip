#include "stdafx.h"

#include "FireBall.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Player/Player.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
    const auto LIFE_TIME = 5.0f;
    const auto HIT_RADIUS = 10.0f;
} // namespace

namespace app
{
    namespace gimmick
    {
        FireBall::~FireBall()
        {
            pEffectEmitter_ = nullptr;
        }


        bool FireBall::Start()
        {
            // Playerを探索。
            pPlayer_ = FindGO<Player>("player");

            // エフェクトを登録。
            EffectEngine::GetInstance()->ResistEffect(app::enemyStatus::EffectID::effect_FireBall,u"Assets/effect/fireBall.efk");

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

            // エフェクト座標同期
            UpdateEffectPosition();

            // 当たり判定
            CheckPlayerCollision();

            // 寿命
            lifeTime_ += deltaTime;
            if (lifeTime_ >= LIFE_TIME)
                DeleteGO(this);
        }


        void FireBall::SetParameter(const Vector3& startPos, const Vector3& dir, float speed)
        {
            position_ = startPos;
            direction_ = dir;
            direction_.Normalize();
            speed_ = speed;
        }


        void FireBall::UpdateEffectPosition()
        {
            if (pEffectEmitter_)
            {
                pEffectEmitter_->SetPosition(position_);
                rotation_.SetRotationYFromDirectionXZ(direction_);
                pEffectEmitter_->SetRotation(rotation_);
            }
        }


        void FireBall::CheckPlayerCollision()
        {
            if (!pPlayer_)
                return;

            Vector3 playerPos = pPlayer_->GetPlayerPos();
            playerPos.y += 20.0f;

            Vector3 diff = playerPos - position_;

            if (diff.LengthSq() <= HIT_RADIUS * HIT_RADIUS)
            {
                pPlayer_->OnDamage(2);
                DeleteGO(this);
            }
        }


        void FireBall::CreateEffect()
        {
            pEffectEmitter_ = NewGO<EffectEmitter>(0);

            if (pEffectEmitter_)
            {
                pEffectEmitter_->Init(app::enemyStatus::EffectID::effect_FireBall);
                pEffectEmitter_->SetPosition(position_);
                pEffectEmitter_->SetScale(Vector3::One);
                pEffectEmitter_->Play();
            }
        }
    } // namespace gimmick
} // namespace app
