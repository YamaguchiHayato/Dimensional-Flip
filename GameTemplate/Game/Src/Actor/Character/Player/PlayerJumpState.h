#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/IState.h"

class Player;


namespace app
{
    namespace state
    {
        class PlayerJumpState : public IState
        {
        private:
            Player* pPlayer_ = nullptr;


        public:
            PlayerJumpState(Player* player) : pPlayer_(player) {};
            virtual ~PlayerJumpState() {};


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            void Move(float speedRate);
            void UpdateJumpAndGravity();
            void ApplyMovement();
        };
    }
}

