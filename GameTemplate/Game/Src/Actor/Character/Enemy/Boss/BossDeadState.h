#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

class Fade;

namespace app
{
    namespace enemyState
    {
        class BossDeadState : public IEnemyState
        {
        public:
            BossDeadState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossDeadState() = default;


        public:
            // 初期化処理。
            void Enter() override;
            // 更新処理。
            void Update() override;
            // ステート終了処理。
            void Exit() override;
            // 次のステートへ遷移要求をする処理。
            bool RequestID(uint8_t& request) override;


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            Fade* pFade_ = nullptr;


        private:
            bool isFadeStarted_ = false;
        };

    }
}

