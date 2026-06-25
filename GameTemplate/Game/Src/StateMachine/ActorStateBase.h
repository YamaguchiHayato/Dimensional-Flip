#pragma once
#include "IActorState.h"

namespace nsApp
{
    namespace nsFunction
    {
        /**
         * @brief Player / Boss / 雑魚敵 共通のステート基底クラス。
         *
         * 既存の RequestID() を残しつつ、
         * IActorState::RequestTransition() に委譲する。
         * StateMachine からは RequestTransition だけ見ればよい。
         */
        class ActorStateBase : public IActorState
        {
        public:
            virtual ~ActorStateBase() = default;

            /**
             * @brief StateMachine から呼ばれる遷移要求。
             */
            bool RequestTransition(uint8_t& outNextId) override { return RequestID(outNextId); }

            /**
             * @brief 既存ステート互換の遷移要求（Player / Enemy 共通）。
             */
            virtual bool RequestID(uint8_t& request) = 0;
        };
    } // namespace nsFunction
} // namespace nsApp
