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
}

namespace nsApp
{
    namespace nsCore
    {
        class Game;
        class InputManager;       //!< 入力マネージャ（ポインタ用 forward decl）
        class BattlePhaseManager; //!< ボス戦足場マネージャ（ポインタ用 forward decl）
    }
}


namespace app
{
    namespace enemy
    {
        class Boss : public IEnemy
        {
        public:
            /* コンストラクタとデストラクタ。*/
            Boss() = default;
            virtual ~Boss();


        public:
            /**
             * @brief 初期化処理。
             * @return 初期化が成功したらtrue、失敗したらfalseを返す。
             */
            bool Start() override;

            /**
             * @brief 更新処理。
             */
            void Update() override;

            /**
             * @brief 描画処理。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;


        public:
            /**
             * @brief アニメーションをロードする。
             * @param animation Animation。
             * @param isLoop ループ設定。
             * @param interPolate 補間値。
             */
            inline void LoadAnimation(app::enemyStatus::BossAnimation animation, bool isLoop, float interPolate)
            {
                animClips_[animation].SetLoopFlag(isLoop);
                render_.PlayAnimation(animation, interPolate);
            }


        // セッター。
        public:
            /**
             * @brief 座標をセットする。
             * @param pos 座標。
             */
            inline void SetPos(const Vector3& pos) override { pos_ = pos; }

            /**
             * @brief 回転をセットする。
             * @param rot 回転。
             */
            inline void SetRot(const Quaternion& rot) { rot_ = rot; }

            /**
             * @brief スケールをセットする。
             * @param scale スケール。
             */
            inline void SetScale(const Vector3& scale) {}

            /**
             * @brief 弱点コリジョンをセットする。
             * @param enable 有効化するかどうか。
             */
            inline void SetStompable(bool enable) {}

            /**
             * @brief 攻撃タイプをセットする
             * @param type 攻撃タイプ
             */
            inline void SetAttackType(app::enemyStatus::AttackType type)
            {
                currentAttackType_ = type;
            }

            /**
             * @brief 次の待機時間をセットする。
             * @param interVal 待機時間
             */
            inline void SettNextInterval(float interVal)
            {
                nextInterval_ = interVal;
            } 

            /**
             * @brief 移動速度をセットする。
             * @param moveSpeed 移動速度
             */
            inline void SetMoveSpeed(const Vector3& moveSpeed)
            {
                moveSpeed_ = moveSpeed;
            }

            /**
             * @brief HPをセットする。
             * @param hp HP
             */
            inline void SetHP(const float hp)
            {
                bossHP_ = hp;
            }

            /**
             * @brief 弱点の高さをセットする。
             * @param height 弱点の高さ。
             * @param overrideBone ボーンの高さを上書きするかどうか
             */
            inline void SetWeakPointHeight(float height, bool overrideBone)
            {
                manualWeakHeight_ = height;
                isManualOverride_ = overrideBone;
            }


        public:
            /**
             * @brief 座標を取得する。
             * @return 座標。
             */
            inline Vector3 GetPos() const
            {
                return pos_;
            }

            /**
             * @brief 回転軸を取得する。
             * @return　回転軸。
             */
            inline Quaternion GetRot() const
            {
                return rot_;
            }

            /**
             * @brief 移動速度を取得する。
             * @return 移動速度。
             */
            inline float GetNextInterval() const
            {
                return nextInterval_;
            }

            /**
             * @brief アニメーションが再生中かどうかを取得する。
             * @return 再生中ならtrue、再生中でなければfalse。
             */
            inline bool IsPlayingAnimation() const
            {
                return render_.IsPlayingAnimation();
            }

            /**
             * @brief プレイヤーのインスタンスを取得する。
             * @return プレイヤーのインスタンス。
             */
            inline Player* GetPlayer() const
            {
                return pPlayer_;
            }

            /**
             * @brief 現在の攻撃タイプを取得する。
             * @return 現在の攻撃タイプ。
             */
            inline app::enemyStatus::AttackType GetAttackType() const
            {
                return currentAttackType_;
            }

            /**
             * @brief 攻撃回数が3回以上かどうかを取得する。
             * @return 3回以上ならtrue、3回未満ならfalse。
             */
            inline bool IsTired() const
            {
               return attackCount_ >= 3;
            }

            /**
             * @brief 攻撃座標をランダムに取得する。
             * @return ランダムな攻撃座標。
             */
            Vector3 GetRandomAttackPos();

            /**
             * @brief 弱点の座標を取得する。
             * @return 弱点の座標。
             */
            Vector3 GetWeakPoint() const;

            /**
             * @brief ゲームインスタンスを取得する。
             * @return ゲームインスタンス
             */
            inline nsApp::nsCore::Game* GetGameInstance() const
            {
                return pGame_;
            }

            /**
             * @brief ボスのHPを取得する。
             * @return ボスのHP
             */
            inline uint8_t GetHP() const
            {
                return bossHP_;
            }

            /**
             * @brief 現在のステートを取得する。
             * @return 現在のステート
             */
            inline app::enemyState::IEnemyState* GetCurrentState() const
            {
                return pCurrentState_;
            }

            /**
             * @brief ステートリストを取得する。
             * @return ステートリスト。
             */
            inline app::enemyState::IEnemyState** GetStateList()
            {
                return pStateList_;
            }

            /**
             * @brief モデルパスを初期化する。
             * @param enemyName 敵の名前。
             * @return モデルパス。
             */
            inline const virtual std::string InitModel(const std::string& enemyName) override
            {
                std::string enemyPath = "Assets/modelData/" + enemyName + ".tkm";
                return enemyPath;
            };


        public:
            /**
             * @brief 攻撃回数を加算する。
             */
            inline void AddAttackCount()
            {
                attackCount_++;
            }

            /**
             * @brief 攻撃回数をリセットする。
             */
            inline void ResetAttackCount()
            {
                attackCount_ = 0;
            }

            /**
             * @brief 弱点のボーンIDをセットする。
             */
            inline void AddClamp();


        private:
            /**
             * @brief アニメーションをセットする。
             */
            void SetAnimation();

            /**
             * @brief ボスの回転を更新する。
             */
            void Rotaition();

            /**
             * @brief ボスの座標をランダムに取得する。
             * @return ランダムなボスの座標。
             */
            Vector3 RandomStagePos();


        private:
            Player* pPlayer_ = nullptr; //! プレイヤーのインスタンス。
            CollisionObject* pWeeekPoint_ = nullptr; //! 弱点のコリジョン。
            nsApp::nsCore::InputManager* pInputManager_ = nullptr; //! 入力マネージャーのインスタンス。
            nsApp::nsCore::Game* pGame_ = nullptr;                 //! ゲームインスタンス。
            CutInView* pCutInView_ = nullptr;      //! カットインビューのインスタンス。


        private:
            app::enemyStatus::AttackType currentAttackType_ =app::enemyStatus::AttackType::Meteor;                                //! 現在の攻撃タイプ。
            AnimationClip animClips_[app::enemyStatus::BossAnimation::bossAnim_Num]; //! アニメーションの種類。
            ModelRender render_;                                                     //! モデルレンダラー。
            Vector3 pos_ = Vector3::Zero;                                            //! ボスの座標。
            Vector3 moveSpeed_ = Vector3::Zero;                                      //! ボスの移動速度。
            Quaternion rot_ = Quaternion::Identity;                                  //! ボスの回転。
            uint8_t state_ = app::enemyStatus::bossAnim_Idle;                        //! 現在のステート。
            uint8_t attackCount_ = 0;                                                //! 攻撃回数。
            uint8_t bossHP_ = 0;                                                     //! ボスのHP。
            bool isAttackSpawned_ = false;                                           //! 攻撃オブジェクトが生成済みかチェック。
            bool isCutInActive;                                                      //! カットインがアクティブかどうか。
            bool isPhasePlaying;                                                     //! フェーズが再生中かどうか。
            bool canBeAttacked_ = true;                                              //! ダメージを受け付けるかどうか。
            bool isManualOverride_ = false;                                          //! 弱点の高さを手動で上書きするかどうか。
            bool isFadeStart_ = false;      //! フェードアウトが開始されたかどうか。
            float stateTimer_ = 0.0f;  //! < ステートのタイマー。
            float nextInterval_ = 3.0f;     //! < 次の待機時間。
            float manualWeakHeight_ = 22.0f; //! < 弱点の高さを手動で上書きする値。
            int weakPointBoneID_ = -1;       //! 弱点のボーンID。


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
