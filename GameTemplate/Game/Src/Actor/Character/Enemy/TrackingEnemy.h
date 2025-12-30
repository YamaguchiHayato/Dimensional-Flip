#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"

// 前方宣言
class Player;

namespace app
{
    namespace enemy
    {
        enum EnEnemyAnimation
        {
            enIdle,
            enWalk,
            enDeath,
            enNum
        };

        class TrackingEnemy : public IEnemy
        {
        public:
            TrackingEnemy() = default;
            virtual ~TrackingEnemy() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            inline void SetPos(const Vector3& pos) override
            {
                pos_ = pos;
                initPos_ = pos;
            }
            inline void SetScale(const Vector3& scale) override { scale_ = scale; }
            inline void SetStompable(bool enable) override { isStompable_ = enable; }

        private:
            void Tracking();       // 索敵・追従
            void Move();           // 物理移動
            void Rotation();       // ★向きの計算
            void Press();          // 踏みつけ演出
            void EnemyAnimation(); // アニメーション制御

            void SetEnemyAnimation();
            const std::string FetchAnimation(EnEnemyAnimation type, const std::string& name, bool loop);

        private:
            Player* pPlayer_ = nullptr;
            ModelRender render_;
            CharacterController charaCon_;
            AnimationClip animationclip_[enNum];

            Vector3 pos_ = Vector3::Zero;
            Vector3 initPos_ = Vector3::Zero;
            Vector3 moveSpeed_ = Vector3::Zero;
            Vector3 scale_ = Vector3(0.25f, 0.25f, 0.25f);
            Quaternion rot_ = Quaternion::Identity;

            // 状態フラグ
            bool isStompable_ = true;
            bool isCrushed_ = false;
            bool isChasing_ = false;
            uint8_t animaState_ = enIdle;
            uint8_t crushedFrame_ = 0;

            // 演出パラメータ
            Vector3 crushStartScale_ = Vector3::Zero;
            Vector3 crushStartPos_ = Vector3::Zero;
            const float CRUSH_SCALE_Y_RATE = 0.2f;
            const float CRUSH_POS_Y_OFFSET = 15.0f;
            const float DETECT_RANGE = 250.0f;
            const float CHASE_SPEED = 2.0f;
        };

    }

} // namespace app::enemy
