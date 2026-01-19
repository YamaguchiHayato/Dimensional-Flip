#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemy
    {
        class BossTumbleState : public IEnemyState
        {
        public:
            BossTumbleState(Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossTumbleState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        public:
            // 弱点ポイントにヒットしたかどうかをチェックする。
            bool CheckWeakPointHit();


        private:
            Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f; // 転倒時間を計測。

            bool canBeAttacked_ = true; // ダメージを受け付けるかどうか。 
        };

    }
}
