#pragma once
#include <cstdint>

#include "NullState.h"
#include "StateMachineCommand.h"

namespace nsApp
{
    namespace nsFunction
    {
        /**
         * @brief ステートの実行・遷移を担当するステートマシン。
         *
         * Template Method:
         *   CanUpdate → OnPreUpdate → 遷移判定 → Update → OnPostUpdate
         *
         * @tparam StateId    ステートID列挙型
         * @tparam StateCount 登録可能な最大ステート数
         */
        template <typename StateId, size_t StateCount> class StateMachine
        {
        public:
            /**
             * @brief コンストラクタ。
             * @param[in] command ステートレジストリ（所有権は呼び出し側）
             */
            explicit StateMachine(StateMachineCommand<StateId, StateCount>& command)
                : command_(command), current_(&NullState::Get()), currentId_(static_cast<StateId>(0))
            {
            }

            virtual ~StateMachine() = default;

            /**
             * @brief 初期ステートで開始する。
             */
            bool Start(StateId initialId) { return ChangeState(initialId); }

            /**
             * @brief 毎フレーム呼ぶ更新処理。
             */
            void Update()
            {
                if (!CanUpdate())
                    return;

                OnPreUpdate();

                uint8_t request = 0;
                if (current_->RequestTransition(request))
                    ChangeState(static_cast<StateId>(request));

                current_->Update();

                OnPostUpdate();
            }

            /**
             * @brief 指定ステートへ強制遷移する。
             */
            bool ChangeState(StateId nextId)
            {
                IActorState* next = command_.Find(nextId);
                if (next == nullptr)
                    return false;

                current_->Exit();
                current_ = next;
                currentId_ = nextId;
                current_->Enter();
                return true;
            }

            /**
             * @brief NullState に戻す。
             */
            void ResetToNull()
            {
                if (current_ != &NullState::Get())
                    current_->Exit();

                current_ = &NullState::Get();
            }

            /**
             * @brief 指定ステート中かどうか。
             */
            bool IsInState(StateId id) const { return currentId_ == id; }

            /**
             * @brief 指定ステート中かどうか。
             * @return true: 指定ステート中, false: それ以外
             */
            IActorState* GetCurrent()
            {
                return current_;
            }

            /**
             * @brief 現在のステートを取得する。
             * @return 現在のステート
             */
            const IActorState* GetCurrent() const
            {
                return current_;
            }


        protected:
            /**
             * @brief 現在のステートIDを取得する。
             * @return 現在のステートID。
             */
            virtual bool CanUpdate() const
            {
                return true;
            }

            /**
             * @brief 現在のステートIDを取得する。
             */
            virtual void OnPreUpdate() {}

            /**
             * @brief 
             */
            virtual void OnPostUpdate() {}


        private:
            StateMachineCommand<StateId, StateCount>& command_; 
            IActorState* current_; 
            StateId currentId_; 
        };
    } // namespace nsFunction
} // namespace nsApp
