#pragma once
#include "stdint.h"

namespace nsApp
{
    namespace nsFunction
    {
        /**
         * @brief 状態遷移を管理するためのインターフェースクラス。
         * @detailこのクラスは、状態遷移を管理するためのインターフェースを提供します。
         * 状態に入る際の処理、更新処理、状態から出る際の処理、および状態遷移の要求を行う関数を定義しています。
         * @date 2026/06/24: 作成日。
         */
        class IActorState
        {
        public:
            /* コンストラクタとデストラクタ。*/
            virtual ~IActorState() = default;


        public:
            /**
             * @brief 状態に入る際の処理を行う関数。
             */
            virtual void Enter() = 0;

            /**
             * @brief 状態の更新処理を行う関数。
             */
            virtual void Update() = 0;

            /**
             * @brief 状態から出る際の処理を行う関数。
             */ 
            virtual void Exit() = 0;

            /**
             * @brief 状態遷移の要求を行う関数。
             * @param outNextID 次の状態IDを格納する変数。
             * @return 状態遷移が要求された場合はtrue、そうでない場合はfalseを返す。
             */
            virtual bool RequestTransition(uint8_t& outNextID) = 0;
        };
    }
}
