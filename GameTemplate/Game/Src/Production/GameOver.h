#pragma once

/**
 * @file   GameOver.h
 * @brief  ゲームオーバー画面のスプライト演出 GO。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class GameOver
         * @brief  GameOver スプライトを全画面表示する。
         */
        class GameOver : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            GameOver() = default;
            virtual ~GameOver() = default;

            /**
             * @brief 初期化。スプライトを読み込む。
             * @return 成功時 true。
             */
            bool Start();

            /**
             * @brief 毎フレームの更新。
             */
            void Update();

            /**
             * @brief 描画。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);


        private:
            SpriteRender sprite_; //!< GameOver スプライト
        };
    } // namespace nsProduction
} // namespace nsApp

using GameOver = nsApp::nsProduction::GameOver;
