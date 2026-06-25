#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"
#include <vector>
#include <memory>

#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Production/CutIn/CutInView.h"
#include "Src/Actor/Character/PlayerForward.h"

namespace app
{
    namespace enemyState
    {
        class BossIdleState;
        class BossAttackState;
        class BossDamageState;
        class BossTumbleState;
        class BossDeadState;
    }

    namespace core
    {
        class Game;
        class InputManager;
        class BattlePhaseManager;
    } 
}



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
            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override;
            // 描画処理。
            void Render(RenderContext& rc) override;




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

            // HPをセット。
            inline void SetHP(const float hp)
            {
                bossHP_ = hp;
            }

            // ステートから弱点コリジョンの高さを上書き。
            inline void SetWeakPointHeight(float height, bool overrideBone)
            {
                manualWeakHeight_ = height;
                isManualOverride_ = overrideBone;
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
//                return attackCount_ >= 3;
                return attackCount_ >= 1;
            }

            // 攻撃クラスの座標計算を取得。
            Vector3 GetRandomAttackPos();

            // 弱点ベクトルのワールド座標を取得する。
            Vector3 GetWeakPoint() const;

            // ゲームクラスを取得。
            inline app::core::Game* GetGameInstance() const
            {
                return pGame_;
            }

            // HPを取得。
            inline uint8_t GetHP() const
            {
                return bossHP_;
            }

            // ボスの現在のステートを取得。
            inline app::enemyState::IEnemyState* GetCurrentState() const
            {
                return pCurrentState_;
            }

            // 現在のボスステートリストを取得。
            inline app::enemyState::IEnemyState** GetStateList()
            {
                return pStateList_;
            }

            // モデルをセット。
            inline const virtual std::string InitModel(const std::string& enemyName) override
            {
                std::string enemyPath = "Assets/modelData/" + enemyName + ".tkm";
                return enemyPath;
            };


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
            // アニメーションをセット。
            void SetAnimation();

            // 回転処理。
            void Rotaition();

            // 座標をランダムに計算するヘルパー関数。
            Vector3 RandomStagePos();


        private:
            Player* pPlayer_ = nullptr;
            CollisionObject* pWeeekPoint_ = nullptr;
            app::core::InputManager* pInputManager_ = nullptr;
            app::core::Game* pGame_ = nullptr;
            app::cutIn::CutInView* pCutInView_ = nullptr;


        private:
            app::enemyStatus::AttackType currentAttackType_ = app::enemyStatus::AttackType::Meteor;
            AnimationClip animClips_[app::enemyStatus::BossAnimation::bossAnim_Num];

            ModelRender render_;

            Vector3 pos_ = Vector3::Zero;
            Vector3 moveSpeed_ = Vector3::Zero;
            Quaternion rot_ = Quaternion::Identity;

            uint8_t state_ = app::enemyStatus::bossAnim_Idle;
            uint8_t attackCount_ = 0;
            uint8_t bossHP_ = 0;


            bool isAttackSpawned_ = false; // 攻撃オブジェクトが生成済みかチェック。
            bool isCutInActive;
            bool isPhasePlaying;
            bool canBeAttacked_ = true;    // ダメージを受け付けるかどうか。
            bool isManualOverride_ = false;
            bool isFadeStart_ = false;

            float stateTimer_ = 0.0f;
            float nextInterval_ = 3.0f;
            float manualWeakHeight_ = 22.0f;

            // @ uint8_tだと負数が扱えないためint型で定義。
            int weakPointBoneID_ = -1;


        // ステート用変数群。
        // フレンドクラス。
        private:
            friend class app::enemyState::BossIdleState;
            friend class app::enemyState::BossAttackState;
            friend class app::enemyState::BossDamageState;
            friend class app::enemyState::BossTumbleState;
            friend class app::enemyState::BossDeadState;


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
