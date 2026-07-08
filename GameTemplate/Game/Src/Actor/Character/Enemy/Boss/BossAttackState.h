#pragma once


#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

#include "Src/Collision/CollisionManager.h"

namespace app
{
    namespace enemyState
    {
        class BossAttackState : public IEnemyState
        {
        public:
            /**
             * @brief コンストラクタ。
             * @param pBoss ボスのインスタンスへのポインタ。
             */
            BossAttackState(app::enemy::Boss* pBoss);
            virtual ~BossAttackState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            // どの攻撃を実行するか決める関数。
            // 3D視点版。
            void DecideStrategy3D();

            // 2D視点版。
            void DecideStrategy2D();

        private:
            app::enemy::Boss* pBoss_ = nullptr;
            CollisionManager* pCollisionManager_ = nullptr;

            std::unique_ptr<app::enemyState::IBossStrategy> currentState_ = nullptr; // 現在の攻撃ステート。


        private:
            bool isPhase2D;

        private:
            uint8_t lastAttackType_ = -1; // 最後に実行した攻撃タイプを保存する変数。
        };

    }
}
