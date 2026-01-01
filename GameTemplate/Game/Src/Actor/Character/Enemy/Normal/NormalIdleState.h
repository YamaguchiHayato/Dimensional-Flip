#pragma once
#include "Src/Actor/Character/Enemy/Normal/NormalEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemy
    {
        class NormalIdleState : public  IEnemyState
        {
        public:
            NormalIdleState(NormalEnemy* pNormal) : pNormal_(pNormal) {}
            virtual ~NormalIdleState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            // 浮遊移動処理。
            void MoveFloating();


        private:
            NormalEnemy* pNormal_ = nullptr;


        private:
            float angle_ = 0.0f;
        };

    }
}

