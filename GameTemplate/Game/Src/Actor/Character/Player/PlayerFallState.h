#pragma once
#include "Src/Actor/Character/IState.h"


namespace app
{
    namespace state
    {
        class PlayerFallState : public IState
        {
        private:
            Player* pPlayer_ = nullptr;


        public:
            PlayerFallState(Player* player) : pPlayer_(player) {};
            virtual ~PlayerFallState() {};


        public:
            void Enter() override {};
            void Update() override;
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            void AirMovement(float moveRate); // 空中での移動処理。
            void ApplyGravity();              // 重力処理。
            void CheckRespawn();              // リスポーンかをチェックする。
            void Respawn();                   // リスポーン処理。
        };
    }
}


