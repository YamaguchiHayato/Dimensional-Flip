#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"


namespace app{
    namespace enemy{
        class NormalEnemy;
    }
} 

namespace app
{
    namespace enemyState
    {
        class NormalChaseState : public IEnemyState
        {
        public:
            NormalChaseState(app::enemy::NormalEnemy* pNormal) : pNormal_(pNormal) {}
            virtual ~NormalChaseState() = default;

            void Enter() override {};
            void Update() override;
            void Exit() override {};
            bool RequestID(uint8_t& request) override;

        private:
            void MoveChase();

        private:
            app::enemy::NormalEnemy* pNormal_ = nullptr;

            // 浮遊用パラメータ (Chaseでも浮遊しながら動くため)
            float angle_ = 0.0f;
        };
    } 
} 
