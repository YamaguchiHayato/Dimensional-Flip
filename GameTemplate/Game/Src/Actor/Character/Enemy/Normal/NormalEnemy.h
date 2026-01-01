#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

// フレンドクラス（各ステートの前方宣言）
namespace app{
    namespace enemy{
        class NormalIdleState;
        class NormalChaseState;
        class NormalCrushedState;
    } 
} 

class Player;

enum  NormalEnemyState : uint8_t
{
    state_Idle,
    state_Chase,
    state_Crushed,
    state_Num,
};

namespace app
{
    namespace enemy
    {
        class NormalEnemy : public IEnemy
        {
        public:
            struct SpawnParam
            {
                Vector3 paramPos_;
                Vector3 paramScale;
                float paramCollision;
                bool paramFlag;

                SpawnParam(const Vector3& pos, const Vector3& scale = Vector3(0.5f, 0.5f, 0.5f), float radius = -1.0f,
                           bool flag = true)
                    : paramPos_(pos), paramScale(scale), paramCollision(radius), paramFlag(flag)
                {
                }
            };

        public:
            NormalEnemy() = default;
            virtual ~NormalEnemy(); // デストラクタ（メモリ解放用）


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        public:
            void InitParam(const SpawnParam& param);
            inline const std::string InitModel(const std::string& enemyName) override
            {
                return IEnemy::InitModel(enemyName);
            }

        // セッター。
        public:
            // 座標を設定。
            inline void SetPos(const Vector3& pos) override
            {
                pos_ = pos;
                initPos_ = pos;
            }

            // スケールを設定。
            inline void SetScale(const Vector3& scale) override
            {
                scale_ = scale;
            }

            // 踏みつけ可能か設定。
            inline void SetStompable(bool enable) override { isStompable_ = enable; }

            // 各種セッター。
            // 現在地を設定。
            inline void SetCurrentPos(const Vector3& pos)
            {
                pos_ = pos;
            }

            // 初期位置を設定。
            inline void SetInitPos(const Vector3& pos)
            {
                initPos_ = pos;
            }

            // 速度を設定。
            inline void SetVelocity(const Vector3& vel)
            {
                velocity_ = vel;
            }


        // ゲッター。
        public:
            // プレイヤーのポインタ取得。
            inline Player* GetPlayer() const
            {
                return pPlayer_;
            }

            // 現在地を取得。
            inline const Vector3& GetCurrentPos() const
            {
                return pos_;
            }

            // 初期位置を取得。
            inline const Vector3& GetInitPos() const
            {
                return initPos_;
            }

            // 速度を取得。
            inline const Vector3& GetVelocity() const
            {
                return velocity_;
            }

            // 踏みつけ可能か取得。
            inline float GetAggroRadius() const
            {
                return aggroRadius_;
            }

            // --- ロジック関数 (IdleState等から呼ぶ) ---
            void UpdateRender();              // 描画更新
            bool TryStomp();                  // 踏みつけ判定
            bool CheckChaseCondition() const; // 追跡判定

            // ステートクラスからのアクセス許可
            friend class NormalIdleState;
            friend class NormalChaseState;
            friend class NormalCrushedState;

        private:
            IEnemyState* pCurrentState_ = nullptr;
            IEnemyState* pStateList_[static_cast<int>(NormalEnemyState::state_Num)] = {nullptr};

            template <typename T> void RegisterState(NormalEnemyState state)
            {
                int index = static_cast<int>(state);
                if (index < static_cast<int>(NormalEnemyState::state_Num))
                {
                    if (pStateList_[index])
                        delete pStateList_[index];
                    pStateList_[index] = new T(this);
                }
            }

        private:
            Vector3 initPos_ = Vector3::Zero;
            Vector3 pos_ = Vector3::Zero;
            Vector3 scale_ = Vector3(0.25f, 0.25f, 0.25f);
            Vector3 velocity_ = Vector3::Zero;

            // 死亡演出用
            Vector3 crushStartPos_ = Vector3::Zero;
            Vector3 crushStartScale_ = Vector3::Zero;

            ModelRender render_;
            Player* pPlayer_ = nullptr;

            bool isStompable_ = true;
            bool isCrushed_ = false;

            float moveSpeed_ = 0.25f;
            float aggroRadius_ = 220.0f;
            float stopRadius_ = 80.0f;
            float accel_ = 0.08f;
            float collisionRadius_ = 80.0f;
            float crushScaleYRate_ = 0.25f;
            float crushPosYOffset_ = 40.0f;
            uint8_t crushedFrame_ = 0;
        };
    } // namespace enemy
} // namespace app
