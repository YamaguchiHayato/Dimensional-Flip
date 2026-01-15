#include "stdafx.h"

#include "Src/Collision/CollisionManager.h"

// ステートクラス。
#include "Src/Actor/Character/Enemy/Fall/FallEnemy.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemyIdleState.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemyFallState.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemyLandState.h"
#include "Src/Actor/Character/Enemy/Fall/FallEnemyRiseState.h"

namespace
{
    const Vector3 SCALE = (Vector3(0.25f, 0.25f, 0.25f));

    const auto RADIUS = 20.0f;
    const auto ROLL_ANGLE = 180.0f;
    const auto DAMEGE_NUM = 2;
}

namespace app
{
    namespace enemy
    {
        FallEnemy::~FallEnemy()
        {
            // 配列に格納したステートを全て削除する。
            for (uint8_t i = 0; i < static_cast<int>(FallEnemyState::state_Num); ++i)
            {
                // 現在のステートが存在する場合。
                if (pCurrentState_)
                {
                    // ステートを削除。
                    delete pStateList_[i];
                    pStateList_[i] = nullptr;
                }
            }

            pCurrentState_ = nullptr;
        }


        bool FallEnemy::Start()
        {
            // ステートの登録。
            // 待機ステート。
            RegisterState<app::enemy::FallEnemyIdleState>(FallEnemyState::state_Idle);
            // 落下ステート。
            RegisterState<app::enemy::FallEnemyFallState>(FallEnemyState::state_Fall);
            // 着地ステート。
            RegisterState<app::enemy::FallEnemyLandState>(FallEnemyState::state_Land);
            // 浮上ステート。
            RegisterState<app::enemy::FallEnemyRiseState>(FallEnemyState::state_Rise);


            // モデルを初期化。
            render_.Init("Assets/modelData/enemy/FallingEnemy.tkm");
            // 初期ステートを待機に設定。
            pCurrentState_ = pStateList_[static_cast<int>(FallEnemyState::state_Idle)];

            // 生成開始位置を初期化。
            startPos_ = position_;

            // 初期ステートの登録。
            if (pCurrentState_)
                pCurrentState_->Enter();

            // モデルの初期更新。
            render_.SetPosition(position_);
            render_.SetScale(SCALE);
            render_.Update();

            return true;
        }


        void FallEnemy::Update()
        {
            // Playerから遠い場合、処理はしない。
            if (IsPlayerNear())
            {
                /////////////////////////////////////////////
                _ASSERT(pCurrentState_ != nullptr);
                // 遷移要求があるか確認。
                uint8_t request = 0;
                if (pCurrentState_->RequestID(request))
                {
                    // 範囲チェック
                    if (request < static_cast<int>(FallEnemyState::state_Num))
                    {
                        pCurrentState_->Exit();
                        pCurrentState_ = pStateList_[request];
                        if (pCurrentState_)
                            pCurrentState_->Enter();
                    }
                }
                /////////////////////////////////////////////
            }

            // ステートの更新。
            if (pCurrentState_)
                pCurrentState_->Update();


            // コリジョンの切り替え更新処理。
            ChangeCollision();

            // モデルの更新。
            rot_.SetRotationDegY(ROLL_ANGLE);
            render_.SetRotation(rot_);
            render_.SetPosition(position_);
            render_.SetScale(SCALE);
            render_.Update();
        }


        void FallEnemy::ChangeCollision()
        {
            if (!pPlayer_)
                return;

            // 現在のコリジョンのモードを取得。
            auto currentCollisionMode = app::collision::CollisionManager::GetInstance().GetCurrentMode();

            // 3Dmodeなら何もしない。
            if (currentCollisionMode == app::collision::DimensionMode::dim3D)
                return;

            // PlayerとFallEnemyの距離を計算。
            // Playerの座標を取得。
            Vector3 playerPos = pPlayer_->GetPlayerPos();

            // FallEnemyの座標を取得。
            Vector3 enemyPos = position_;

            // 距離を計算する。
            Vector3 finalDistance = playerPos - enemyPos;

            if (currentCollisionMode == app::collision::DimensionMode::dim2D)
                // 2Dmodeの当たり判定。
                finalDistance.z = 0.0f;


            // 距離判定。
            auto distSq = finalDistance.LengthSq();
            if (distSq < (RADIUS * RADIUS))
                // ダメージを与える。
                // この敵は2ダメージを与える。
                pPlayer_->OnDamage(DAMEGE_NUM);
        }


        void FallEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
