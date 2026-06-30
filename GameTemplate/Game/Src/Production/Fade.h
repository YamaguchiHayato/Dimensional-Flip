#pragma once

#include "Src/Actor/Stage/StageID.h"

/**
 * @file   Fade.h
 * @brief  画面フェードイン/アウト演出 GO。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class Fade
         * @brief ワイプ付きフェード。シーン遷移とステージ遷移の両方で使用。
         */
        class Fade : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/ 
            Fade();
            virtual ~Fade() = default;

        public:
            /**
             * @brief フェード状態。
             */
            void Update();

            /**
             * @brief フェード描画。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);

            /**
             * @brief フェードアウトを開始する。
             */
            void StartFadeOut();

            /**
             * @brief フェードインを開始する。
             */
            void StartFadeIn();

            /**
             * @brief フェード状態を取得する。
             * @return  フェードの状態を返す。
             */
            inline FadeState GetFadeState() const { return fadeState_; }

            /**
             * @brief フェード用スプライトレンダラーを取得する。
             * @return フェード用スプライトレンダラーを返す。
             */
            inline SpriteRender& GetFadeSprite() { return fadeSprite_; }

            /**
             * @brief フェードイン終了フラグを取得する。
             * @return フェードイン終了フラグを返す。
             */
            inline bool IsFadeInEnd() const  { return fadeInEnd_; }

            /**
             * @brief フェードアウト終了フラグを取得する。
             * @return フェードアウト終了フラグを返す。
             */
            inline bool IsFadeOutEnd() const { return fadeOutEnd_; }


        private:
            /**
             * @brief フェード状態を変更する。
             */
            void ChangeFadeState();


        private:
            FadeState fadeState_ = FadeState::Fade_In; //! < フェード状態
            SpriteRender fadeSprite_;                  //! < フェード用スプライトレンダラー
            Sprite sprite_;                            //! < フェード用スプライト
            uint8_t fadeCounter_ = 0;                  //! < フェードカウンター
            bool fadeInEnd_ = false;                   //! < フェードイン終了フラグ
            bool fadeOutEnd_ = false;                  //! < フェードアウト終了フラグ
            bool fadeTransitionFlag_ = false;          //! < フェード遷移中フラグ
            bool loadingFlag_ = false;                 //! < ローディング中フラグ
            bool transitionInProgress_ = false;        //! < 遷移中フラグ
        };
    } // namespace nsProduction
} // namespace nsApp

using Fade = nsApp::nsProduction::Fade;
