#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}


namespace app
{
    namespace enemyState
    {
        class BossAttackJumpState : public IBossStrategy
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossAttackJumpState() =default;
            virtual ~BossAttackJumpState() =default;


        private:
            /**
             * @brief ステートに入るときに呼ばれる。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss)override;

            /**
             * @brief ステートを更新する。
             */
            void Update() override;

            /**
             * @brief ステートから抜けるときに呼ばれる。
             */
            void Exit() override;

            /**
             * @brief ステートが終了したかどうかを返す。
             * @return trueなら終了している。
             */
            bool IsFinished() const override;


        private:
            /**
             * @brief ステートを更新する。
             */
            void UpdateState();

            /**
             * @brief Prepare状態。
             */
            void UpdatePrepare();

            /**
             * @brief Jumping状態。
             */
            void UpdateJumping();

            /**
             * @brief Landing状態。
             */
            void UpdateLanding();

            /**
             * @brief エフェクトを初期化する。
             */
            void InitEffect();

            /**
             * @brief プレイヤーとの当たり判定をチェックする。
             * @return trueなら当たった。
             */
            bool CheckPlayerCollision();


        private:
            /**
             * @brief ジャンプ回数を追加する。
             */
            inline void AddJumpCount()
            {
                jumopCount_++;
            }


        private:
            app::enemy::Boss* pBoss_ = nullptr;                                     //! ボスのポインタ。
            app::enemyStatus::JumpStep step_ = app::enemyStatus::JumpStep::Prepare; //! ジャンプのステップ。


        private:
            Vector3 startPos_ = Vector3::Zero;                                      //! ジャンプ開始位置。
            Vector3 targetPos_ = Vector3::Zero;                                     //! ジャンプ目標位置。
            Vector3 velocity_ = Vector3::Zero;                                      //! ジャンプの速度。 
            Quaternion faceAngle_ = Quaternion::Identity;                           //! ジャンプの向き。
            float timer_ = 0.0f;                                                    //! ジャンプのタイマー。
            int jumopCount_ = 3;                                                    //! ジャンプ回数。
            bool hasHitPlayer_ = false;                                             //! プレイヤーに当たったかどうか。
        };
    }
}

