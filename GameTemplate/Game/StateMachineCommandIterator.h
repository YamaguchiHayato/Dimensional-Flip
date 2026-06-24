#pragma once
#include <cstddef>
#include <utility>

namespace nsApp
{
    namespace nsFunction    
    {
        class IActorState;

        template <typename StateId, size_t StateCount> class StateMachineCommand;

        /**
         * @brief StateMachineCommand に登録済みのステートだけを走査する Iterator。
         *
         * 未登録（nullptr）のスロットはスキップする。
         * range-for や ForEach の基盤として使用する。
         *
         * @tparam StateId    ステートを識別する列挙型（uint8_t ベース想定）
         * @tparam StateCount ステート配列の最大数
         */
        template <typename StateId, size_t StateCount> class StateMachineCommandIterator
        {
        public:
            using Command = StateMachineCommand<StateId, StateCount>;
            using value_type = std::pair<StateId, IActorState*>;
            using difference_type = std::ptrdiff_t;

            StateMachineCommandIterator() = default;

            /**
             * @brief イテレータを生成する。
             * @param[in] owner 走査対象の Command
             * @param[in] index 開始インデックス
             */
            StateMachineCommandIterator(const Command* owner, size_t index) : owner_(owner), index_(index)
            {
                // 未登録スロットを飛ばし、登録済み位置で止まる。
                AdvanceToRegistered();
            }

            /**
             * @brief 現在位置の (StateId, IActorState*) を返す。
             */
            value_type operator*() const
            {
                return {static_cast<StateId>(index_), owner_->GetAt(index_)};
            }

            /**
             * @brief 次の登録済みスロットへ進む。
             */
            StateMachineCommandIterator& operator++()
            {
                ++index_;
                AdvanceToRegistered();
                return *this;
            }

            /**
             * @brief 等価比較（end 判定用）。
             */
            bool operator!=(const StateMachineCommandIterator& other) const
            {
                return index_ != other.index_;
            }


        private:
            /**
             * @brief 未登録スロットを飛ばし、登録済み位置で止まる。
             */
            void AdvanceToRegistered()
            {
                /* nullの場合は何もしない。*/
                if (!owner_)
                    return;

                /* 未登録スロットはスキップ。*/
                while (index_ < owner_->GetCapacity() && !owner_->IsRegisteredAt(index_))
                    ++index_;
            }


        private:
            const Command* owner_ = nullptr; //! 走査対象 Command。
            size_t index_ = 0;               //! 現在の配列インデックス。
        };
    } // namespace core
} // namespace app
