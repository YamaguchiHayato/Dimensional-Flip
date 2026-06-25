#pragma once
#include <cstdint>

#include "IActorState.h"

namespace nsApp
{
    namespace nsFunction    
    {
        /**
         * @brief 何もしないステート（Null Object）。
         *
         * 用途:
         *   - StateMachine 開始前の current_ 先として使う
         *   - 「無効なステート」を明示したいときのプレースホルダ
         *   - 遷移先未設定時の安全なフォールバック
         *
         * @note シングルトン。コピー・代入不可。
         */
        class NullState : public IActorState
        {
        public:
            /**
             * @brief シングルトンインスタンスを返す。
             */
            static NullState& Get()
            {
                static NullState instance;
                return instance;
            }

            /**
             * @brief 何もしない。
             */
            void Enter() override {}

            /**
             * @brief 何もしない。
             */
            void Update() override {}

            /**
             * @brief 何もしない。
             */
            void Exit() override {}

            /**
             * @brief 常に false（遷移要求なし）。
             */
            bool RequestTransition(uint8_t& /*outNextId*/) override { return false; }


        private:
            /* コンストラクタとデストラクタ。*/
            NullState() = default;
            virtual ~NullState() = default;


        private:
            /* コピー&ムーブを禁止する。*/
            NullState(const NullState&) = delete;
            NullState& operator=(const NullState&) = delete;
        };
    } // namespace core
} // namespace app
