#pragma once

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace
{
    auto FALL_SPEED = 80.0f;
    auto RISE_SPEED = 150.0f;
    auto MOVE_DISTANCE = 0.0f;
    auto WAIT_BOTTOM_TIME = 0.5f;
}

namespace app{
    namespace enemyState
    {
        class FallEnemyIdleState;
        class FallEnemyFallState;
        class FallEnemyLandState;
        class FallEnemyRiseState;
    } // namespace enemyState
}

class Player;

// 落下する敵クラス。
namespace app
{
    namespace enemy
    {
        // ステート識別する列挙型。
        enum  FallEnemyState : uint8_t
        {
            state_Idle, // 待機。
            state_Fall, // 落下。
            state_Land, // 着地。
            state_Rise, // 浮上。
            state_Num,  // 総数。
        };


        // 落下する敵クラス。
        class FallEnemy : public IEnemy
        {
        public:
            FallEnemy() = default;
            virtual ~FallEnemy();


        // 継承処理。
        public:
            bool Start()override;
            void Update()override;
            void Render(RenderContext& rc)override;
            inline const virtual std::string InitModel(const std::string& enemyName)
            {
                return IEnemy::InitModel(enemyName);
            };


        private:
            // 当たり判定を切り替える。
            void ChangeCollision();


        // セッター。
        public:
            // 座標をセット。
            inline void SetPos(const Vector3& pos) override
            {
                position_ = pos;
                startPos_ = pos;
            }

            // 踏みつけて倒せるかどうか。
            inline void SetStompable(bool enable) override {};

            // 大きさ。
            inline void SetScale(const Vector3& scale) override
            {
                scale_ = scale;
            }

            // プレイヤーのポインタをセット。
            inline void SetPlayer(Player* pPlayer)
            {
                pPlayer_ = pPlayer;
            }

            // 起動時間の設定。
            inline void SetStartUpDelay(float delatTime)
            {
                startUpDelay_ = delatTime;
            }

            // 2D時の起動ライン(X座標)。
            inline void SetTriggerX(const float x)
            {
                triggerX_ = x;
            }

        // ゲッター。
        public:
            // 死亡判定。
            inline bool IsDead() const
            {
                return false;
            }

            // Playerの距離判定。
            inline bool IsPlayerNear() const
            {
                if (!pPlayer_)
                    return false ;

                // 距離を計算。
                Vector3 diff = pPlayer_->GetPlayerPos() - GetStartPos();
                diff.z = 0.0f;

                float detectionRadius = 2000.0f;

                return diff.LengthSq() < (detectionRadius * detectionRadius);
            };

            // 開始位置を取得。
            inline const Vector3& GetStartPos()const
            {
                return startPos_;
            }

            // 待機時間の取得。
            inline float GetWaitTopTime()
            {
                return waitTopTime_;
            }

            // 落下速度
            inline float GetFallSpeed() const
            {
                return FALL_SPEED;
            }

            // 上昇速度
            inline float GetRiseSpeed() const
            {
                return RISE_SPEED;
            } 

            // 落下距離（初期位置からどれくらい下がるか）
            inline float GetMoveDistance() const
            {
                return MOVE_DISTANCE;
            }

            // 着地後の待機時間
            inline float GetWaitBottomTime() const
            {
                return WAIT_BOTTOM_TIME;
            }

            // 座標の取得。
            inline const Vector3& GetPosition()const
            {
                return position_;
            }

            // 起動時間の取得。
            inline const float GetStartUpDelay()const
            {
                return startUpDelay_;
            }

            // トリガー座標の取得。
            inline const float GetTriggerX()const
            {
                return triggerX_;
            }

            // Playerの情報を取得。
            inline  Player* GetPlayer()
            {
                return pPlayer_;
            }

        private:
            ModelRender render_;
            Vector3 startPos_ = Vector3::Zero;
            Vector3 position_ = Vector3::Zero;
            Vector3 scale_ = Vector3::One;

            Quaternion rot_ = Quaternion::Identity;


        private:
            float waitTopTime_ = 0.1f;  // 落下前の待機時間。
            float startUpDelay_ = 0.0f; // 起動遅延時間。
            float triggerX_ = 0.0f;     // 2D時の起動ライン(X座標)。


        private:
            Player* pPlayer_ = nullptr;


        // ステート用変数群。
        // フレンドクラス。
        private:
            friend class app::enemyState::FallEnemyIdleState;
            friend class app::enemyState::FallEnemyFallState;
            friend class app::enemyState::FallEnemyLandState;
            friend class app::enemyState::FallEnemyRiseState;


        // ステート登録用テンプレート関数。
        private:
            template <typename T> void RegisterState(app::enemy::FallEnemyState state)
            {
                int index = static_cast<int>(state);
                if (index < static_cast<int>(app::enemy::FallEnemyState::state_Num))
                {
                    // 古いステートは削除。
                    if (pStateList_[index])
                        delete pStateList_[index];


                    // ステートを生成して配列に保持させる。
                    pStateList_[index] = new T(this);
                }
            }


        // ステート変数群。
        private:
            app::enemyState::IEnemyState* pCurrentState_ = nullptr;
            app::enemyState::IEnemyState* pStateList_[static_cast<int>(app::enemy::FallEnemyState::state_Num)] = {nullptr};

        };

    }
}
