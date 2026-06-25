#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Player/Context/IPlayerStateContext.h"
#include "Src/StateMachine/ActorStateBase.h"

namespace nsApp
{
    namespace nsState
    {
        /**
         * @brief プレイヤー待機ステート。
         */
        class PlayerIdleState : public nsFunction::ActorStateBase
        {
        private:
            IPlayerStateContext& context_; ///< State が触る唯一の窓口。

        public:
            /**
             * @brief コンストラクタ。
             * @param[in,out] context プレイヤー Context。
             */
            explicit PlayerIdleState(IPlayerStateContext& context) : context_(context) {}
            virtual ~PlayerIdleState() = default;

        public:
            /** @brief ステート開始時の処理。 */
            void Enter() override;

            /** @brief ステート更新処理。 */
            void Update() override;

            /** @brief ステート終了時の処理。 */
            void Exit() override {}

            /**
             * @brief 遷移要求を判定する。
             * @param[out] request 遷移先ステート ID。
             * @return 遷移する場合 true。
             */
            bool RequestID(uint8_t& request) override;
        };
    } // namespace nsState
} // namespace nsApp
