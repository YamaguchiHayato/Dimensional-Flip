#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"
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
        // ステート管理用の列挙型。
        enum BossState : uint8_t
        {
            state_Idle = 0, // 待機。
            state_Attack,   // 攻撃。
            state_Hit,      // ダメージ。
            state_Tumble,   // 疲労。
            state_Num       // ステートの総数。
        };


        class Boss : public IEnemy
        {
        public:
            Boss() = default;
            virtual ~Boss();

        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;
            inline const virtual std::string InitModel(const std::string& enemyName) override
            {
                std::string enemyPath = "Assets/modelData/" + enemyName + ".tkm";
                return enemyPath;
            };


        public:
            // どのアニメーションを再生するか設定する。
            inline void LoadAnimation(BossAnimation animation, bool isLoop, float interPolate)
            {
                render_.PlayAnimation(animation, interPolate);
            }

            // 攻撃ギミックを生成する。
            void SpawnGimmicks(AttackType type);

        // セッター。
        public:
            // 座標。
            inline void SetPos(const Vector3& pos) override { pos_ = pos; }

            // 回転。
            inline void SetRot(const Quaternion& rot) { rot_ = rot; }

            // 大きさをセット。
            inline void SetScale(const Vector3& scale) {}

            // 踏みつけて倒せるかどうかをセット。
            inline void SetStompable(bool enable) {}

            // ボスの攻撃タイプをセットする。
            inline void SetAttackType(AttackType type)
            {
                currentAttackType_ = type;
            }

            // 待機時間をセット。
            inline void SettNextInterval(float interVal)
            {
                nextInterval_ = interVal;
            } 


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

            // 待機時間。
            inline float GetNextInterval() const
            {
                return nextInterval_;
            }

            // アニメーションが再生中か。
            inline bool IsPlayingAnimation() const
            {
                return render_.IsPlayingAnimation();
            }

            // プレイヤーを取得。
            inline Player* GetPlayer() const
            {
                return pPlayer_;
            }

            // 攻撃タイプ。
            inline AttackType GetAttackType() const
            {
                return currentAttackType_;
            }

            // 疲労状態に入るか。
            inline bool IsTired() const
            {
                return attackCount_ >= 6;
            }

        public:
            // 攻撃回数を加算数する。
            inline void AddAttackCount()
            {
                attackCount_++;
            }

            // 攻撃回数をリセットする。
            inline void ResetAttackCount()
            {
                attackCount_ = 0;
            }

        private:
            void SetAnimation();
            void Rotaition();

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

        // ステート用変数群。
        // フレンドクラス。
        private:
            friend class BossIdleState;
            friend class BossAttackState;
            friend class BossDamageState;
            friend class BossTumbleState;


        // ステート管理用関数群。
        private:
            template <typename T> void RegisterState(uint8_t request)
            {
                int index = static_cast<int>(request);
                if (index < static_cast<int>(BossState::state_Num))
                {
                    // 古いステートは削除。
                    if (pStateList_[index])
                        delete pStateList_[index];

                    // ステートを生成して配列に保持させる。
                    pStateList_[index] = new T(this);
                }
            }


        // ステート管理用変数。
        private:
            IEnemyState* pCurrentState_ = nullptr;
            IEnemyState* pStateList_[static_cast<int>(BossState::state_Num)] ={ nullptr };
        };
    }
}
