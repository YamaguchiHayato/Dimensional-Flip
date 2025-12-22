#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include <vector>
#include <memory>

enum BossAnimation : uint8_t
{
    bossAnim_Idle = 0,      // 待機。
    bossAnim_AttackCast,    // 攻撃。
    bossAnim_AttackRoar,    // 咆哮。
    bossAnim_Tumble,        // 転倒。
    bossAnim_Run,           // 走る。
    bossAnim_Hit,           // ダメージ時。
    bossAnim_Num,           // アニメーションの総数。
};

// 攻撃タイプを管理する列挙型。
enum class AttackType : uint8_t
{
    Meteor, // 隕石
    Spear,  // 槍
    Roar,   // 咆哮
    Num
};

namespace app
{
    namespace gimmick
    {
        class FloatingPlatform;
    }
}

class Player;

namespace app
{
    namespace enemy
    {
        class Boss : public IEnemy
        {
        public:
            Boss() = default;
            virtual ~Boss() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;
            inline const virtual std::string InitModel(const std::string& enemyName) override
            {
                std::string enemyPath = "Assets/modelData/" + enemyName + ".tkm";
                return enemyPath;
            };

            // セッター。
        public:
            // 座標。
            inline void SetPos(const Vector3& pos)override
            {
                pos_ = pos;
            }


            // 回転。
            inline void SetRot(const Quaternion& rot)
            {
                rot_ = rot;
            }


            // 大きさをセット。
            inline void SetScale(const Vector3& scale) {}


            // 踏みつけて倒せるかどうかをセット。
            inline void SetStompable(bool enable) {}


            // ゲッター。
        public:
            // 座標。
            inline Vector3 GetPos() const
            {
                return pos_;
            }


            // 回転。
            inline Quaternion GetRot() const
            {
                return rot_;
            }


        private:
            void ControlState();
            void PlayAnimation();
            void SetAnimation();
            void Rotaition();
            // 攻撃Gimmickをまとめて生成させる。
            void SpawnGimmicks(AttackType type);

            // 足場を生成するヘルパー。
            void SpawnPlatforms();

            // 座標をランダムに計算するヘルパー関数。
            Vector3 RandomStagePos();

        private:
            Player* pPlayer_ = nullptr;
            std::vector<app::gimmick::FloatingPlatform*> pFloatingPlatform_;
            CollisionObject* pWeeekPoint_ = nullptr;

        private:
            AttackType currentAttackType_ = AttackType::Meteor;
            ModelRender render_;

            Vector3 pos_ = Vector3::Zero;
            Vector3 moveSpeed_ = Vector3::Zero;
            Quaternion rot_ = Quaternion::Identity;

            uint8_t state_ = bossAnim_Idle;
            uint8_t attackCount_ = 0;
            uint8_t hp = 3;

            bool isAttackSpawned_ = false; // 攻撃オブジェクトが生成済みかチェック。
            bool canBeAttacked_ = true;    // ダメージを受け付けるかどうか。

            float stateTimer_ = 0.0f;
            float nextInterval_ = 3.0f;
        };
    }
}
