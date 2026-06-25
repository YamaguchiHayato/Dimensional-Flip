#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/IState.h"
#include "Src/Actor/Character/PlayerForward.h"



namespace nsApp
{
    namespace nsState
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
        };
    }
}

