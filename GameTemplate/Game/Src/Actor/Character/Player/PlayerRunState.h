#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/IState.h"


class Player;
class CameraManager;

namespace app
{
    namespace state
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
            void CalculateRunMovement(); // 移動計算
            void ApplyMovement();        // 物理反映
        };

    }
    
}

