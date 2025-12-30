#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"

class Player;

enum enNormalEnemyAnimation : uint8_t
{
    ghost_Idle,
    ghost_Death,
    ghost_Num,
};

namespace app
{
    namespace enemy
    {
        class NormalEnemy : public IEnemy
        {
        public:
            // 生成時のパラメータ構造体。
            struct SpawnParam
            {
                Vector3 paramPos_;
                Vector3 paramScale;
                float paramCollision;
                bool paramFlag;

                // デフォルト値を設定。
                SpawnParam(const Vector3& pos, const Vector3& scale = Vector3(0.5f, 0.5f, 0.5f), float radius = -1.0f,
                           bool flag = true)
                    : paramPos_(pos), paramScale(scale), paramCollision(radius), paramFlag(flag)
                {
                }
            };

        public:
            NormalEnemy() = default;
            virtual ~NormalEnemy() = default;

            // エンジンの基本関数。
        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            // 独自の関数。
        public:
            void Press();
            void InitParam(const SpawnParam& param);
            inline const std::string InitModel(const std::string& enemyName) override
            {
                return IEnemy::InitModel(enemyName);
            };

        public:
            // 座標をセット。
            inline void SetPos(const Vector3& pos) override
            {
                pos_ = pos;
                initPos_ = pos;
            }

            // モデルごとに大きさを設定する。
            inline void SetScale(const Vector3& scale) override { scale_ = scale; }

            // 敵を踏みつけて倒せるかどうか判定する。
            inline void SetStompable(bool enable) override { isStompable_ = enable; }

        private:
            // 浮遊移動処理。
            void MoveFloating();

        private:
            Vector3 initPos_ = Vector3::Zero;
            Vector3 pos_ = Vector3::Zero;
            Vector3 scale_ = Vector3(0.25f, 0.25f, 0.25f);
            Vector3 crushStartPos_ = Vector3::Zero;
            Vector3 crushStartScale_ = Vector3::Zero;
            ModelRender render_;

            bool isStompable_ = true;
            bool isCrushed_ = false;

            float angle_ = 0.0f;      // 浮遊の角度。
            float spped_ = 0.05f;     // 上下の速度。
            float range_ = 10.0f;     // 上下の幅。
            float moveSpeed_ = 0.25f; // 最大速度（1フレームあたりの移動量）
            Vector3 velocity_ = Vector3::Zero;
            float aggroRadius_ = 220.0f; // この距離以内で追いかけ開始（範囲を狭める）
            float stopRadius_ = 80.0f;   // この距離以内では止まる（張り付き防止）
            float accel_ = 0.08f;        // 速度のなめらかさ（小さいほど急に加速しない）

            // ★近接判定用（踏める距離など）
            float collisionRadius_ = 80.0f;

            // ★潰れ演出の調整
            float crushScaleYRate_ = 0.25f; // Yを何倍に潰すか(0..1)
            float crushPosYOffset_ = 40.0f; // 潰れる時に下へずらす量

            uint8_t crushedFrame_ = 0;

        private:
            Player* pPlayer_ = nullptr;
        };

    } // namespace enemy
} // namespace app
