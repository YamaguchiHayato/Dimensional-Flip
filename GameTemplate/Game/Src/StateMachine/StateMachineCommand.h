#pragma once
#include <array>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <utility>

#include "IActorState.h"
#include "StateMachineCommandIterator.h"

namespace nsApp
{
    namespace nsFunction
    {
        /**
         * @brief ステートの生成・破棄・探索を担当するレジストリ。
         *
         * ステートの実行・遷移は StateMachine 側の責務。
         * Player / Boss / Enemy はこのクラスをメンバに持ち、
         * 重複していた RegisterState / delete ループをここに集約する。
         *
         * @tparam StateId    ステートID列挙型
         * @tparam StateCount 登録可能な最大ステート数
         */
        template <typename StateId, size_t StateCount> class StateMachineCommand
        {
        public:
            using Iterator = StateMachineCommandIterator<StateId, StateCount>;

            /// @brief ファクトリ関数の型（RegisterFactory 用）
            using Factory = std::unique_ptr<IActorState> (*)();

            StateMachineCommand() = default;

            /**
             * @brief デストラクタ。全ステートを破棄する。
             * @note 既存 Boss デストラクタの delete ループ相当。
             */
            ~StateMachineCommand() { ClearAll(); }


        public:
            /* コピー&ムーブを禁止する。*/
            StateMachineCommand(const StateMachineCommand&) = delete;
            StateMachineCommand& operator=(const StateMachineCommand&) = delete;


        public:
            // ============================================================
            // Iterator（登録済みスロットのみ）
            // ============================================================

            /**
             * @brief 先頭イテレータ（登録済みの最初の要素）。
             */
            Iterator begin() const { return Iterator(this, 0); }

            /**
             * @brief 終端イテレータ。
             */
            Iterator end() const { return Iterator(this, StateCount); }

            /**
             * @brief 登録済みステートを列挙する。
             * @tparam Func (StateId id, IActorState* state) を受け取る関数
             */
            template <typename Func> void ForEach(Func&& func) const
            {
                for (const auto& entry : *this)
                    func(entry.first, entry.second);
            }

            // ============================================================
            // 生成・登録
            // ============================================================

            /**
             * @brief ステートを生成して登録する。
             *
             * 同じ ID が既に登録されている場合は、先に破棄してから上書きする。
             * 既存 Boss::RegisterState の挙動と同等。
             *
             * @tparam StateT 生成するステート型（IActorState 実装）
             * @tparam Args   StateT コンストラクタ引数
             * @param[in] id  登録先ステートID
             * @return 登録成功なら true
             */
            template <typename StateT, typename... Args> bool Emplace(StateId id, Args&&... args)
            {
                const size_t index = ToIndex(id);
                if (index >= StateCount)
                    return false;

                // 既存があれば先に破棄
                states_[index].reset();
                states_[index] = std::make_unique<StateT>(std::forward<Args>(args)...);
                return states_[index] != nullptr;
            }

            /**
             * @brief ファクトリ関数でステートを登録する。
             * @param[in] id      登録先ステートID
             * @param[in] factory 生成関数
             * @return 登録成功なら true
             */
            bool RegisterFactory(StateId id, Factory factory)
            {
                const size_t index = ToIndex(id);
                if (index >= StateCount || !factory)
                    return false;

                states_[index].reset();
                states_[index] = factory();
                return states_[index] != nullptr;
            }

            /**
             * @brief 既存の unique_ptr を所有権ごと登録する。
             * @param[in] id    登録先ステートID
             * @param[in] state 登録するステート
             * @return 登録成功なら true
             */
            bool RegisterUnique(StateId id, std::unique_ptr<IActorState> state)
            {
                const size_t index = ToIndex(id);
                if (index >= StateCount || !state)
                    return false;

                states_[index] = std::move(state);
                return true;
            }

            // ============================================================
            // 破棄
            // ============================================================

            /**
             * @brief 指定IDのステートを破棄する。
             * @param[in] id 破棄するステートID
             * @return 破棄できたら true
             */
            bool Remove(StateId id)
            {
                const size_t index = ToIndex(id);
                if (index >= StateCount)
                    return false;

                states_[index].reset();
                return true;
            }

            /**
             * @brief 全ステートを破棄する。
             */
            void ClearAll()
            {
                for (auto& state : states_)
                    state.reset();
            }


            // ============================================================
            // 探索
            // ============================================================

            /**
             * @brief 指定IDのステートを取得する。
             * @param[in] id ステートID
             * @return 見つかればポインタ、なければ nullptr
             */
            IActorState* Find(StateId id)
            {
                const size_t index = ToIndex(id);
                if (index >= StateCount)
                    return nullptr;

                return states_[index].get();
            }

            /**
             * @brief 指定IDのステートを取得する（const版）。
             */
            const IActorState* Find(StateId id) const
            {
                return const_cast<StateMachineCommand*>(this)->Find(id);
            }

            /**
             * @brief 指定IDのステートが登録済みか。
             */
            bool Contains(StateId id) const
            {
                return Find(id) != nullptr;
            }

            /**
             * @brief 登録済みステート数を返す。
             */
            size_t CountRegistered() const
            {
                size_t count = 0;
                for (const auto& state : states_)
                {
                    if (state)
                        ++count;
                }
                return count;
            }

            /**
             * @brief 1つも登録されていないか。
             */
            bool IsEmpty() const
            {
                return CountRegistered() == 0;
            }

            /**
             * @brief 全スロットが埋まっているか。
             */
            bool IsFull() const
            {
                return CountRegistered() == StateCount;
            }

            /**
             * @brief 必須ステートがすべて登録済みか（Start 前検証用）。
             * @param[in] requiredIds 必須IDリスト
             */
            bool HasAll(std::initializer_list<StateId> requiredIds) const
            {
                for (StateId id : requiredIds)
                {
                    if (!Contains(id))
                        return false;
                }
                return true;
            }

            // ============================================================
            // Iterator 用走査 API
            // ============================================================

            /**
             * @brief 配列の最大容量（StateCount）を返す。
             */
            size_t GetCapacity() const
            {
                return StateCount;
            }

            /**
             * @brief 指定インデックスにステートが登録されているか。
             * @note Iterator から使用。
             */
            bool IsRegisteredAt(size_t index) const
            {
                return index < StateCount && states_[index] != nullptr;
            }

            /**
             * @brief 指定インデックスのステートを返す。
             * @note Iterator から使用。未登録なら nullptr。
             */
            IActorState* GetAt(size_t index)
            {
                if (index >= StateCount)
                    return nullptr;

                return states_[index].get();
            }

            /**
             * @brief 指定インデックスのステートを返す（const版）。
             */
            const IActorState* GetAt(size_t index) const
            {
                if (index >= StateCount)
                    return nullptr;

                return states_[index].get();
            }


        private:
            /**
             * @brief StateId を配列インデックスに変換する。
             */
            static size_t ToIndex(StateId id)
            {
                return static_cast<size_t>(id);
            }


        private:
            std::array<std::unique_ptr<IActorState>, StateCount> states_{}; //! ステート所有配列。
        };
    } // namespace core
} // namespace app
