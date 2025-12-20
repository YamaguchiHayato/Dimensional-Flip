#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/IState.h"

class Player;


namespace app
{
    namespace state
    {
        class PlayerIdleState : public IState
        {
        private:
            Player* pPlayer_ = nullptr;


        public:
            PlayerIdleState(Player* player) : pPlayer_(player) {};
            virtual ~PlayerIdleState() {};


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            void ApplyMovement();
        };

    }
}

