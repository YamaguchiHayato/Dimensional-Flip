#pragma once

/**
 * @file   EndRollBase.h
 * @brief  エンドロール構成要素の基底クラス。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class EndRollBase
         * @brief  EndRollImage / EndRollText 等の共通インターフェース。
         */
        class EndRollBase : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            EndRollBase() = default;
            virtual ~EndRollBase() = default;

        public:
            /**
             * @brief 初期化処理。
             * @return 成功時 true。
             */
            virtual bool Start() = 0;

            /**
             * @brief 更新処理。
             */
            virtual void Update() = 0;

            /**
             * @brief 描画処理。
             * @param rc レンダリングコンテキスト。
             */
            virtual void Render(RenderContext& rc) = 0;


        public:
            /**
             * @brief 終了判定。
             * @return 終了していれば true。
             */
            inline bool IsEnd() const { return isEnd_; }


        protected:
            bool isEnd_ = false; //!< 終了フラグ
        };
    } // namespace nsProduction
} // namespace nsApp

using EndRollBase = nsApp::nsProduction::EndRollBase;
