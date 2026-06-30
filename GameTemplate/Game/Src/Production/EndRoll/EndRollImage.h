#pragma once
#include "Src/Production/EndRoll/EndRollBase.h"

/**
 * @file   EndRollImage.h
 * @brief  エンドロール左側のスライドショー画像。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class EndRollImage
         * @brief  一定間隔で切り替わるスタッフロール画像。
         */
        class EndRollImage : public EndRollBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            EndRollImage() = default;
            virtual ~EndRollImage() = default;


        public:
            /**
             * @brief 初期化処理。スライドショー画像を構築する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。一定時間ごとに画像を切り替える。
             */
            void Update() override;

            /**
             * @brief 描画処理。現在の画像を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;


        private:
            /**
             * @brief スライドショー画像を初期化する。
             */
            void InitSlideShow();


        private:
            std::vector<std::unique_ptr<SpriteRender>> images_; //!< スライドショー画像のリスト
            int currentIndex_ = 0; //!< 現在表示中の画像インデックス
            float timer_ = 0.0f;     //!< 切り替えタイマー
        };
    } // namespace nsProduction
} // namespace nsApp

using EndRollImage = nsApp::nsProduction::EndRollImage;
