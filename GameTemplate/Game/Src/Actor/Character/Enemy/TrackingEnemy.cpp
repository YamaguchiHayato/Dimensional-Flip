#include "stdafx.h"

#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    static constexpr uint8_t kCrushFrames = 6;
    static constexpr uint8_t kCrushWaitFrames = 10;


    struct StompParam
    {
        // 踏まれる判定を司る距離。
        static constexpr float RADIUS = 6.0f; 
        // Y座標の最小値。
        static constexpr float MIN_Y = 0.0f;
        // Y座標の最大値。
        static constexpr float MAX_Y = 50.0f;
    };
} // namespace

namespace app
{
    namespace enemy
    {
        bool TrackingEnemy::Start()
        {
            SetEnemyAnimation();
            // enNum を使用
            render_.Init("Assets/modelData/Skeleton.tkm", animationclip_, enNum, enModelUpAxisZ, true);
            render_.SetScale(scale_);
            charaCon_.Init(2.0f, 1.0f, pos_);
            initPos_ = pos_;
            pPlayer_ = FindGO<Player>("player");
            return true;
        }


        void TrackingEnemy::Update()
        {
            if (isCrushed_)
            {
                Press();
                return;
            }

            if (!pPlayer_)
                return;

            Tracking();
            Move();
            Rotation(); 
            EnemyAnimation();

            // 踏みつけ判定 (NormalEnemyと共通)
            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;


            // Playerが落下中のみ判定を行う。
            if (isStompable_ && pPlayer_->GetMoveSpeed().y < 0.0f)
            {
                // XZ成分の距離の2乗。
                // → それぞれの成分の2乗を加算。 
                auto distXZ = diff.x * diff.x + diff.z * diff.z;
                auto dy = diff.y;

                // 直径。
                auto diameter = StompParam::RADIUS * StompParam::RADIUS;


                if (distXZ <= diameter && dy > StompParam::MIN_Y && dy < StompParam::MAX_Y)
                {
                    // Playerにバウンド処理をさせる。
                    pPlayer_->Bound();

                    pPlayer_->AddScore(100);

                    isCrushed_ = true;
                    crushedFrame_ = 0;
                    crushStartScale_ = scale_;
                    crushStartPos_ = pos_;
                    charaCon_.RemoveRigidBoby();
                    return;
                }
            }


            if (!isCrushed_ && pPlayer_)
            {
                Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
                float distXZSq = diff.x * diff.x + diff.z * diff.z;

                // 半径 6.0f (6*6 = 36) 以内
                if (distXZSq < (6.0f * 6.0f))
                {

                    if (diff.y < 5.0f && diff.y > -20.0f)
                    {
                        pPlayer_->OnDamage(1);
                    }
                }
            }
            render_.SetPosition(pos_);
            render_.SetRotation(rot_); // 計算した回転をセット
            render_.SetScale(scale_);
            render_.Update();
        }

        void TrackingEnemy::Tracking()
        {
            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
            if (!isChasing_ && diff.Length() < DETECT_RANGE)
            {
                isChasing_ = true;
                animaState_ = enWalk;
            }

            if (isChasing_)
            {
                diff.y = 0.0f;
                if (diff.LengthSq() > 1.0f)
                {
                    diff.Normalize();
                    moveSpeed_.x = diff.x * CHASE_SPEED;
                    moveSpeed_.z = diff.z * CHASE_SPEED;
                }
            }
        }

        void TrackingEnemy::Move()
        {
            if (charaCon_.IsOnGround())
                moveSpeed_.y = 0.0f;
            else
                moveSpeed_.y -= 9.8f * GameTime().GetFrameDeltaTime();
            pos_ = charaCon_.Execute(moveSpeed_, GameTime().GetFrameDeltaTime());
        }


        void TrackingEnemy::Rotation()
        {
            Quaternion correctionRot = Quaternion::Identity;
            // 2. 進行方向に向く回転（Y軸回転）
            Quaternion facingRot = Quaternion::Identity;

            // 追跡中（移動中）なら、移動方向の角度を計算
            if (isChasing_ && (moveSpeed_.x != 0.0f || moveSpeed_.z != 0.0f))
            {
                // XとZの速度から角度(ラジアン)を算出
                float angle = atan2f(moveSpeed_.x, moveSpeed_.z);

                // Y軸周りの回転を作成
                facingRot.SetRotation(Vector3::AxisY, angle);
            }
            else
            {
                // 追跡していない時も、初期位置などで特定の方向を向かせたい場合は
                // ここで facingRot を設定します。現在はそのままでOK。
            }

            // 3. 回転の合成（補正 × 方向）
            // 順番が重要です。まずモデルを起こして(correction)、それから回す(facing)
            rot_ = correctionRot * facingRot;

            // 描画クラスに適用
            render_.SetRotation(rot_);
       }

        void TrackingEnemy::Press()
        {
            crushedFrame_++;
            float t = (float) crushedFrame_ / (float) kCrushFrames;
            if (t > 1.0f)
                t = 1.0f;
            scale_.y = crushStartScale_.y * (1.0f + (CRUSH_SCALE_Y_RATE - 1.0f) * t);
            pos_.y = crushStartPos_.y - (CRUSH_POS_Y_OFFSET * t);
            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.Update();
            if (crushedFrame_ >= kCrushFrames + kCrushWaitFrames)
                DeleteGO(this);
        }

        void TrackingEnemy::EnemyAnimation()
        {
            render_.PlayAnimation(animaState_, 0.2f);
        }

        const std::string TrackingEnemy::FetchAnimation(EnEnemyAnimation type, const std::string& name, bool loop)
        {
            std::string path = "Assets/animData/" + name + ".tka";
            animationclip_[type].Load(path.c_str());
            animationclip_[type].SetLoopFlag(loop);
            return path;
        }

        void TrackingEnemy::SetEnemyAnimation()
        {
            FetchAnimation(enIdle, "SkeletonIdle", true);
            FetchAnimation(enWalk, "SkeletonWalk", true);
            FetchAnimation(enDeath, "SkeletonDeath", false);
        }

        void TrackingEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }

    }

} // namespace app::enemy
