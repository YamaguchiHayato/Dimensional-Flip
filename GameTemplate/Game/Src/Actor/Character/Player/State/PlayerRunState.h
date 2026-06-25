#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/IState.h"
#include "Src/Actor/Character/PlayerForward.h"


class CameraManager;

namespace nsApp
{
    namespace nsState
    {
        class PlayerRunState : public IState
        {
        private:
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;


        public:
            PlayerRunState(Player* player) : pPlayer_(player) {};
            virtual ~PlayerRunState() {};


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        private:
            void CalculateRunMovement(); 
            void ApplyMovement();                   
            void UpdatePlayerMove();


        private:
            float currentTime_ = 0.0f;
        };
    }
    
}

