#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackMeteoState : public IBossStrategy
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossAttackMeteoState() = default;
            virtual ~BossAttackMeteoState() = default;


        public:
            /**
             * @brief ボスの攻撃状態に入るときに呼ばれる関数。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss)override;

            /**
             * @brief ボスの攻撃状態の更新処理。
             */
            void Update() override;

            /**
             * @brief ボスの攻撃状態から抜けるときに呼ばれる関数。
             */
            void Exit() override;

            /**
             * @brief ボスの攻撃状態が終了したかどうかを判定する関数。
             * @return 
             */
            bool IsFinished() const override;


        private:
            /**
             * @brief ボスの攻撃を生成する関数。
             */
            void CreateMeteoAttack();


        private:
            app::enemy::Boss* pBoss_ = nullptr; //! ボスのポインタ。


        private:
            float timer_ = 0.0f;                //! 攻撃のタイマー。
            bool isAttackSpawned_ = false;      //! 攻撃が生成されたかどうかのフラグ。
        };
    }
}

