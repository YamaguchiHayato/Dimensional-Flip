#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

namespace app
{
    namespace enemyState
    {
        class BossAttackMeteo2DState : public IBossStrategy
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossAttackMeteo2DState() = default;
            virtual ~BossAttackMeteo2DState() = default;


        public:
            /**
             * @brief ボスの攻撃状態に入る。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss) override;

            /**
             * @brief ボスの攻撃状態を更新する。
             */
            void Update()override;

            /**
             * @brief ボスの攻撃状態を終了する。
             */
            void Exit()override;


        public:
            /**
             * @brief ボスの攻撃状態が終了したかどうかを取得する。
             * @return 終了している場合はtrue、そうでない場合はfalse。
             */
            inline bool IsFinished() const override
            {
                return isFinished_;
            }


        private:
            /**
             * @brief ボスの攻撃を生成する。
             */
            void SpawnMeteoLine();


        private:
            app::enemy::Boss* pBoss_ = nullptr; //! ボスのポインタ。


        private:
            float timer_ = 0.0f;                //! 攻撃状態のタイマー。
            bool isAttackSpawned_ = false;      //! 攻撃が生成されたかどうかのフラグ。
            bool isFinished_ = false;           //! 攻撃状態が終了したかどうかのフラグ。
        };
    }
}

