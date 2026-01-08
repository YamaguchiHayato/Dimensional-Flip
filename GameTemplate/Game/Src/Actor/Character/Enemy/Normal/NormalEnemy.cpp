#include "stdafx.h"

#include "NormalEnemy.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Collision/CollisionManager.h"

// 各ステートのヘッダ
#include "NormalIdleState.h"
#include "NormalChaseState.h" 
#include "NormalCrushedState.h"

namespace
{
    static constexpr uint8_t kCrushFrames = 6;
    static constexpr uint8_t kCrushWaitFrames = 10;
} // namespace

namespace app
{
    namespace enemy
    {
        NormalEnemy::~NormalEnemy()
        {
            // メモリ解放
            for (auto* state : pStateList_)
            {
                if (state)
                    delete state;
            }
        }

        bool NormalEnemy::Start()
        {
            render_.Init("Assets/modelData/enemy/umbrella_yellow.tkm");
            render_.SetPosition(pos_);
            render_.Update();

            pPlayer_ = FindGO<Player>("player");

            // --- ステート登録 ---
            RegisterState<NormalIdleState>(NormalEnemyState::state_Idle);
            RegisterState<NormalChaseState>(NormalEnemyState::state_Chase);
            RegisterState<NormalCrushedState>(NormalEnemyState::state_Crushed);

            // 初期ステート開始
            pCurrentState_ = pStateList_[static_cast<int>(NormalEnemyState::state_Idle)];
            if (pCurrentState_)
                pCurrentState_->Enter();

            return true;
        }


        void NormalEnemy::Update()
        {
            if (!pCurrentState_)
                return;

            // --- ステートマシン駆動 (Player.cpp参考) ---
            uint8_t request = 0;
            if (pCurrentState_->RequestID(request))
            {
                // 範囲チェック
                if (request < static_cast<int>(NormalEnemyState::state_Num))
                {
                    pCurrentState_->Exit();
                    pCurrentState_ = pStateList_[request];
                    if (pCurrentState_)
                        pCurrentState_->Enter();
                }
            }

            if (!isCrushed_ && pPlayer_)
            {
                Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
                float distXZSq = diff.x * diff.x + diff.z * diff.z;
                if (distXZSq < (8.0f * 8.0f))
                {
                    if (diff.y < 5.0f && diff.y > -40.0f)
                    {
                        pPlayer_->OnDamage(1);
                    }
                }

            }

            // 現在のステート更新
            if (pCurrentState_)
                pCurrentState_->Update();
        }


        void NormalEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }

\
        void NormalEnemy::UpdateRender()
        {
            render_.SetPosition(pos_);
            render_.SetScale(scale_);
            render_.Update();
        }


        bool NormalEnemy::TryStomp()
        {
            if (!pPlayer_ || !isStompable_)
                return false;

            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
            Vector3& v = pPlayer_->GetMoveSpeed();

            // プレイヤーが落下中のみ判定
            if (v.y < -0.1f)
            {
                const float dy = diff.y;
                const float stompRadius = 20.0f;
                float distXZSq = diff.x * diff.x + diff.z * diff.z;

                if (dy > 0.0f && dy < 30.0f && distXZSq <= stompRadius * stompRadius)
                {
                    v.y = 60.0f; // プレイヤーをバウンドさせる
                    pPlayer_->Bound();

                    // スコア加算
                    pPlayer_->AddScore(100); 
                    return true;
                }
            }
            return false;
        }


        bool NormalEnemy::CheckChaseCondition() const
        {
            using app::collision::CollisionManager;
            using app::collision::DimensionMode;

            // 3Dモード以外は追わない
            if (CollisionManager::GetInstance().GetCurrentMode() != DimensionMode::dim3D)
                return false;

            if (!pPlayer_)
                return false;

            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
            diff.y = 0.0f;

            // 索敵範囲内ならtrue
            return (diff.LengthSq() <= aggroRadius_ * aggroRadius_);
        }


        void NormalEnemy::InitParam(const SpawnParam& param)
        {
            pos_ = param.paramPos_;
            initPos_ = param.paramPos_;
            scale_ = param.paramScale;
            if (param.paramCollision > 0.0f)
                collisionRadius_ = param.paramCollision;
            isStompable_ = param.paramFlag;
            velocity_ = Vector3::Zero;
            render_.SetPosition(pos_);
            render_.SetScale(scale_);
        }
    } 
}
