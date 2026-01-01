#include "stdafx.h"

#include "NormalEnemy.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Collision/CollisionManager.h"

namespace
{
    static constexpr uint8_t kCrushFrames = 6;      // 潰れるまでのフレーム数
    static constexpr uint8_t kCrushWaitFrames = 10; // 潰れたまま残すフレーム数
} // namespace

namespace app
{
    namespace enemy
    {
        bool NormalEnemy::Start()
        {
            render_.Init("Assets/modelData/enemy/umbrella_yellow.tkm");

            // 座標をセット。
            render_.SetPosition(pos_);
            // 更新処理。
            render_.Update();

            pPlayer_ = FindGO<Player>("player");

            if (scale_.x > 1.0f)
            {
                Vector3 collisionSize(100.0f, 100.0f, 100.0f);
                Vector3 offset(0.0f, 50.0f, 0.0f);

                // （今のプロジェクトでは当たり判定作成処理がここに無いので、そのまま残し）
            }

            return true;
        }


        void NormalEnemy::Update()
        {
            if (isCrushed_)
            {
                Press();
                return;
            }

            // --- 通常時 ---
            if (!pPlayer_)
                return;

            // 浮遊移動処理（3D時のみ追従）
            MoveFloating();

            // --- 踏み判定（プレイヤーの下方向速度で「踏んだ」を判定してる前提の既存ロジック） ---
            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;

            // プレイヤー速度（参照で取る：踏んだ瞬間に書き換えるため）
            Vector3& v = pPlayer_->GetMoveSpeed();

            // 落下中だけ
            if (v.y < -0.1f && isStompable_)
            {
                const float dy = diff.y;

                // 「上から踏む」高さ範囲（調整ノブ）
                const float stompMinY = 0.0f;
                const float stompMaxY = 30.0f;

                // XZだけ小さく判定（調整ノブ：小さいほど一括が起きにくい）
                const float stompRadius = 3.0f;
                const float distXZSq = diff.x * diff.x + diff.z * diff.z;

                if (dy > stompMinY && dy < stompMaxY && distXZSq <= stompRadius * stompRadius)
                {
                    v.y = 60.0f; 

                    // バウンド処理。
                    pPlayer_->Bound();

                    isCrushed_ = true;
                    crushedFrame_ = 0;
                    crushStartScale_ = scale_;
                    crushStartPos_ = pos_;

                    return; // この敵はここで終了
                }
            }
            render_.SetPosition(pos_);
            render_.SetScale(scale_);
            render_.Update();
        }

        void NormalEnemy::MoveFloating()
        {
            // --- 上下の浮遊（Y）---
            angle_ += spped_;

            // 0..1 の範囲にして「下方向に潜らない」（床貫通対策）
            float y01 = (sinf(angle_) + 1.0f) * 0.5f;
            float yOffset = y01 * range_;

            pos_.y = initPos_.y + yOffset;

            // --- 3D時のみ追従（XZ）---
            using app::collision::CollisionManager;
            using app::collision::DimensionMode;

            if (CollisionManager::GetInstance().GetCurrentMode() != DimensionMode::dim3D)
            {
                // 2D時は追従しない
                velocity_ *= 0.8f;
                return;
            }

            if (!pPlayer_)
                return;

            Vector3 toPlayer = pPlayer_->GetPlayerPos() - pos_;
            toPlayer.y = 0.0f;

            const float distSq = toPlayer.LengthSq();
            const float aggroSq = aggroRadius_ * aggroRadius_;
            const float stopSq = stopRadius_ * stopRadius_;

            Vector3 desiredVel = Vector3::Zero;

            // 範囲内だけ追う + 近すぎたら止まる（突進＆張り付き防止）
            if (distSq <= aggroSq && distSq > stopSq && distSq > 0.0001f)
            {
                const float dist = sqrtf(distSq);
                toPlayer.Normalize();

                float denom = (aggroRadius_ - stopRadius_);
                if (denom < 1.0f)
                    denom = 1.0f;

                float t = (dist - stopRadius_) / denom; // 0..1
                if (t < 0.0f)
                    t = 0.0f;
                else if (t > 1.0f)
                    t = 1.0f;

                // イージング：遠いほど速い / 近いほど遅い（突進しにくい）
                float eased = t * t;

                desiredVel = toPlayer * (moveSpeed_ * eased);
            }

            // 速度を滑らかに追従（突進防止の本命）
            velocity_ += (desiredVel - velocity_) * accel_;

            // 位置更新（XZのみ）
            pos_.x += velocity_.x;
            pos_.z += velocity_.z;

            // 浮遊基準も追従（XZだけ）
            initPos_.x = pos_.x;
            initPos_.z = pos_.z;
        }

        void NormalEnemy::Press()
        {
            // 潰れ始め
            crushedFrame_++;

            // 潰れアニメーション（前半 kCrushFrames で潰す）
            if (crushedFrame_ <= kCrushFrames)
            {
                float t = static_cast<float>(crushedFrame_) / static_cast<float>(kCrushFrames);

                // スケール計算（Yだけ潰す）
                const float startY = crushStartScale_.y;
                const float endY = crushStartScale_.y * crushScaleYRate_;
                scale_.y = startY + (endY - startY) * t;

                // 座標計算（沈み込み）
                const float startPosY = crushStartPos_.y;
                const float endPosY = crushStartPos_.y - crushPosYOffset_;
                pos_.y = startPosY + (endPosY - startPosY) * t;

                render_.SetScale(scale_);
                render_.SetPosition(pos_);
                render_.Update();
                return;
            }

            // 潰れ終わった後：当たり判定を消す
            collisionRadius_ = 0.0f;

            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.Update();

            // アニメーション完了＋待機時間が過ぎたら削除
            if (crushedFrame_ >= kCrushFrames + kCrushWaitFrames)
                DeleteGO(this);
        }

        void NormalEnemy::InitParam(const SpawnParam& param)
        {
            pos_ = param.paramPos_;
            initPos_ = param.paramPos_;

            scale_ = param.paramScale;

            if (param.paramCollision > 0.0f)
            {
                collisionRadius_ = param.paramCollision;
            }

            isStompable_ = param.paramFlag;

            // 生成時に挙動リセット（生成直後の暴走防止）
            velocity_ = Vector3::Zero;
            angle_ = 0.0f;
            isCrushed_ = false;
            crushedFrame_ = 0;

            render_.SetPosition(pos_);
            render_.SetScale(scale_);
        }

        void NormalEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    } // namespace enemy
} // namespace app
