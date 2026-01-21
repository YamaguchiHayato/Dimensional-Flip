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
    namespace enemyState
    {
        class NormalCrushedState : public IEnemyState
        {
        public:
            NormalCrushedState(app::enemy::NormalEnemy* pNormal) : pNormal_(pNormal) {}
            virtual ~NormalCrushedState() = default;

            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;

        private:
            app::enemy::NormalEnemy* pNormal_ = nullptr;
        };
    } // namespace enemy
} // namespace app
