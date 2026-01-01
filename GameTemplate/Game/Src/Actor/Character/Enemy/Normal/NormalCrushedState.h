#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemy
    {
        class NormalEnemy;
    }
} // namespace app

namespace app
{
    namespace enemy
    {
        class NormalCrushedState : public IEnemyState
        {
        public:
            NormalCrushedState(NormalEnemy* pNormal) : pNormal_(pNormal) {}
            virtual ~NormalCrushedState() = default;

            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;

        private:
            NormalEnemy* pNormal_ = nullptr;
        };
    } // namespace enemy
} // namespace app
