#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	// キャラコンの定数。
	const float RADIUS = 25.0f;
	const float HEIGHT = 100.0f;

	// EnemyStateの定数。
	const float ENEMYSTATE_ZERO = 270.0f;
	const float ENEMYSTATE_ONE = 90.0f;
	const float ENEMYSTATE_TWO = -180.0f;

	// スケール。
	const Vector3 SCALE(5.0f, 5.0f, 5.0f);
}

namespace app
{
    namespace enemy
    {
        // アニメーションメソッド。
        const ::std::string TrackingEnemy::FetchAnimation(EnEnemyAnimation enemyAnimation,
                                                          const std::string& animationName, bool flag)
        {
            std::string Animation = ENEMY_ANIMATION + animationName + ANIMATION_FILE_EXTENSION;

            // アニメーションのロード。
            animationclip_[enemyAnimation].Load(Animation.c_str());
            animationclip_[enemyAnimation].SetLoopFlag(flag);

            return Animation;
        }

        // 初期化処理。
        bool TrackingEnemy::Start()
        {

            // アニメーションの設定。
            SetEnemyAnimation();

            // モデルの初期化。
            render_.Init("Assets/modelData/Skeleton.tkm", animationclip_, Num, enModelUpAxisY, true);

            // 大きさをセット。
            render_.SetScale(SCALE);

            // キャラコン。
            charaCon_.Init(RADIUS, HEIGHT, pos_);

            // 座標をセット。
            render_.SetPosition(pos_);

            // 更新処理。
            render_.Update();

            // エフェクトの初期化。
            //	EffectEngine::GetInstance()->ResistEffect(EffectList_EnemyHit, u"Assets/effect/enemyhiteffect.efk");

            // 探索処理。
            pPlayer_ = FindGO<Player>("player");

            return true;
        }

        // 更新処理。
        void TrackingEnemy::Update()
        {
            // 追跡処理。
            Tracking();

            // 動作処理。
            Move();

            // 回転処理。
            Rotation();

            // アニメーション。
            EnemyAnimation();

            // モデルをセット。
            render_.SetPosition(pos_);

            // 更新処理。
            render_.Update();

            Vector3 diff = pPlayer_->pos_ - pos_;
            if (diff.Length() <= 100.0f && animaState_ != 1)
            {
                if (pPlayer_->charaCon_.IsOnGround() == false)
                {
                    animaState_ = 1;
                    pPlayer_->moveSpeed_.y = 500.0f;
                    charaCon_.RemoveRigidBoby();

                    ////エフェクトの処理
                    // EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
                    // effectEmitter->Init(EffectList_EnemyHit);

                    ////エフェクトの位置の設定
                    // Vector3 enemyLocalPos = { 0.0f,120.0f,0.0f };
                    // enemyLocalPos += m_position;
                    // effectEmitter->SetPosition(enemyLocalPos);

                    ////エフェクトの大きさの設定
                    // effectEmitter->SetScale({ 30.0f,30.0f,30.0f });

                    ////エフェクトの再生
                    // effectEmitter->Play();

                    ////プレイヤーが敵を踏んだ時の音を再生。
                    // g_gameSoundEngine->PlaySE(GameSoundList_SE_Player_StepOnEnemy, 1.0f);
                }
                else
                {
                    // プレイヤーに触れた
                    touchPlayerFlag_ = true;
                }
            }
        }

        // 動作処理。
        void TrackingEnemy::Move()
        {
            if (!isChasing_)
            {
                if (animaState_ == 0)
                {
                    moveSpeed_.x = -2.0f;
                }
                else if (animaState_ == 1)
                {
                    moveSpeed_.x = 2.0f;
                }
                if (pos_.x >= initPos_.x + 200.0f)
                {
                    animaState_ = 0;
                }
                else if (pos_.x <= initPos_.x - 200.0f)
                {
                    animaState_ = 1;
                }
            }

            if (animaState_ == 1)
            {
                moveSpeed_.x = 0;
                if (!render_.IsPlayingAnimation())
                {
                    DeleteGO(this);
                }
            }
            pos_ = charaCon_.Execute(moveSpeed_, 1.0f);
            float glavity = 3.0f;
            if (charaCon_.IsOnGround())
            {
                // 重力をなくす
                moveSpeed_.y = 0.0f;
            }
            moveSpeed_.y -= glavity;
        }

        // 追跡処理。
        void TrackingEnemy::Tracking()
        {
            Vector3 diff = pPlayer_->pos_ - pos_;
            const bool inRadius = (diff.Length() < 100.0f);

            // B) 座標トリガ（敵があるX座標を越えたら）
            const bool passX = (pos_.x >= triggerX_);

            if (!isChasing_ && (inRadius || passX))
            {
                isChasing_ = true;
            }

            // ---- 追跡中の移動ベクトルを設定 ----
            if (isChasing_)
            {
                diff.y = 0.0f; // 水平面だけで追う（必要ならYも可）
                if (diff.LengthSq() > 1e-4f)
                {
                    diff.Normalize();
                    moveSpeed_.x = diff.x * chaseSpeed_;
                    moveSpeed_.z = diff.z * chaseSpeed_;
                }

                else
                    moveSpeed_.x = moveSpeed_.z = 0.0f; // ほぼ重なったら停止
            }
        }

        // 回転処理。
        void TrackingEnemy::Rotation()
        {
            if (animaState_ == EnEnemyAnimation::enIdle)
                rot_.SetRotationDegY(ENEMYSTATE_ZERO);

            else if (animaState_ == EnEnemyAnimation::enDeath)
                rot_.SetRotationDegY(ENEMYSTATE_ONE);

            rot_.AddRotationDegX(ENEMYSTATE_TWO);
            // 絵描きさんに回転を教える。
            render_.SetRotation(rot_);
        }

        // Enemyのアニメーション。
        void TrackingEnemy::EnemyAnimation()
        {
            switch (animaState_)
            {
            case EnEnemyAnimation::enIdle:
                render_.PlayAnimation(EnEnemyAnimation::enIdle, 0.1f);
                break;

            case EnEnemyAnimation::enDeath:
                render_.PlayAnimation(EnEnemyAnimation::enDeath, 0.1f);
                break;
            }
        }

        // 描画処理。
        void TrackingEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }

        // アニメーションの再生。
        void TrackingEnemy::SetEnemyAnimation()
        {
            // 待機モーション。
            FetchAnimation(EnEnemyAnimation::enIdle, "SkeletonIdle", true);

            // 歩きモーション。
            FetchAnimation(EnEnemyAnimation::enWalk, "SkeletonWalk", true);

            // 死亡モーション。
            FetchAnimation(EnEnemyAnimation::enDeath, "SkeletonDeath", false);
        }

    }
}

