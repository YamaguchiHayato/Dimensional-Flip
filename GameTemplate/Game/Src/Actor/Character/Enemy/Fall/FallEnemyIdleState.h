#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"

#include "Src/Actor/Character/Enemy/Fall/FallEnemy.h"

namespace app
{
    namespace enemy
    {
        class FallEnemyIdleState : public IEnemyState
        {
        public:
            FallEnemyIdleState(FallEnemy* pFallEnemy) : pFallEnemy_(pFallEnemy) {};
            virtual ~FallEnemyIdleState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override {};
            bool RequestID(uint8_t& request) override;


        private:
            // タイマー設定。
            inline void SettingTimer(float timer, bool isTimeOver)
            {
                // タイマーをセット。
                timer_ = timer;
                // タイムアップフラグをセット。
                isTimeOver_ = isTimeOver;
            }


        private:
            FallEnemy* pFallEnemy_ = nullptr;


        private:
            float timer_ = 0.0f;      // 計測タイマー。
            bool isTimeOver_ = false; // 時間切れフラグ。
        };
    }
}
