#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"
#include <vector>
#include <memory>

#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app
{
    namespace enemyState
    {
        class BossIdleState;
        class BossAttackState;
        class BossDamageState;
        class BossTumbleState;
    }

    namespace core
    {
        class Game;
        class InputManager;
        class BattlePhaseManager;
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
            inline void LoadAnimation(app::enemyStatus::BossAnimation animation, bool isLoop, float interPolate)
            {
                // セットしたアニメーションのLoop設定をAnimationClipに適応させる
                animClips_[animation].SetLoopFlag(isLoop);

                render_.PlayAnimation(animation, interPolate);
            }


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
            inline void SetAttackType(app::enemyStatus::AttackType type)
            {
                currentAttackType_ = type;
            }

            // 待機時間をセット。
            inline void SettNextInterval(float interVal)
            {
                nextInterval_ = interVal;
            } 

            // 移動速度をセット。
            inline void SetMoveSpeed(const Vector3& moveSpeed)
            {
                moveSpeed_ = moveSpeed;
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
            inline app::enemyStatus::AttackType GetAttackType() const
            {
                return currentAttackType_;
            }

            // 疲労状態に入るか。
            inline bool IsTired() const
            {
                return attackCount_ >= 3;
            }

            // 攻撃クラスの座標計算を取得。
            Vector3 GetRandomAttackPos();

            // 弱点ベクトルのワールド座標を取得する。
            Vector3 GetWeakPoint() const
            {
                // ボーンが見つかっていない場合は固定位置を返す。
                if (weakPointBoneID_ == -1)
                    return pos_ + Vector3(0.0f, 22.0f, 0.0f);

                // ボーンのワールド行列を取得。
                const Matrix& mat = render_.GetBone(weakPointBoneID_)->GetWorldMatrix();

                // 位置を取得。
                Vector3 pos;
                // ID。
                pos.Set(mat.v[3]); // Skeleton.cppの実装に準拠
                return pos;
            }


            // ゲームクラスを取得。
            inline app::core::Game* GetGameInstance() const
            {
                return pGame_;
            }


        // ヘルパー。
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

            // カメラの範囲外にボスモデルが行かないようにクランプ。
            inline void AddClamp();


        private:

            void SetAnimation();
            void Rotaition();

            // 座標をランダムに計算するヘルパー関数。
            Vector3 RandomStagePos();


        private:
            Player* pPlayer_ = nullptr;
            CollisionObject* pWeeekPoint_ = nullptr;
            app::core::InputManager* pInputManager_ = nullptr;
            app::core::Game* pGame_ = nullptr;


        private:
            app::enemyStatus::AttackType currentAttackType_ = app::enemyStatus::AttackType::Meteor;
            AnimationClip animClips_[app::enemyStatus::BossAnimation::bossAnim_Num];

            ModelRender render_;

            Vector3 pos_ = Vector3::Zero;
            Vector3 moveSpeed_ = Vector3::Zero;
            Quaternion rot_ = Quaternion::Identity;

            uint8_t state_ = app::enemyStatus::bossAnim_Idle;
            uint8_t attackCount_ = 0;
            uint8_t hp = 3;

            bool isAttackSpawned_ = false; // 攻撃オブジェクトが生成済みかチェック。
            bool canBeAttacked_ = true;    // ダメージを受け付けるかどうか。

            float stateTimer_ = 0.0f;
            float nextInterval_ = 3.0f;

            // @ uint8_tだと負数が扱えないためint型で定義。
            int weakPointBoneID_ = -1;

        // ステート用変数群。
        // フレンドクラス。
        private:
            friend class app::enemyState::BossIdleState;
            friend class app::enemyState::BossAttackState;
            friend class app::enemyState::BossDamageState;
            friend class app::enemyState::BossTumbleState;


        // ステート管理用関数群。
        private:
            template <typename T> void RegisterState(uint8_t request)
            {
                int index = static_cast<int>(request);
                if (index < static_cast<int>(app::enemyStatus::BossState::state_Num))
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
            app::enemyState::IEnemyState* pCurrentState_ = nullptr;
            app::enemyState::IEnemyState* pStateList_[static_cast<int>(app::enemyStatus::BossState::state_Num)] = { nullptr};
        };
    }
}
