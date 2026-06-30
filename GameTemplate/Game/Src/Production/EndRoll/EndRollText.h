#pragma once
#include "Src/Production/EndRoll/EndRollBase.h"

/**
 * @file   EndRollText.h
 * @brief  スタッフロールテキストのスクロール表示。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class EndRollText
         * @brief  縦スクロールのスタッフロールテキスト。
         */
        class EndRollText : public EndRollBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            EndRollText() = default;
            virtual ~EndRollText() = default;

        public:
            /**
             * @brief 初期化処理。スタッフロールテキストを構築する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。スクロールを進める。
             */
            void Update() override;

            /**
             * @brief 描画処理。スクロール中のテキストを描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief スキップ処理（スクロールを即終了）。
             */
            void Skip() { isEnd_ = true; }


        private:
            /**
             * @brief スタッフロールテキストの 1 行を表す構造体。
             */
            struct StaffRollLine
            {
                FontRender staffRollText_; //! < スタッフロールテキストのフォントレンダラー
                Vector3 initializePos_ = Vector3::Zero; //! < 初期位置。
            };

            std::vector<std::unique_ptr<StaffRollLine>> lines_;
            float scrollSpeed_ = 1.0f;

            /**
             * @brief スタッフロールテキストのスクロールを管理する。
             * @return スクロールが終了したら true。
             */
            bool ManageText();
        };
    } // namespace nsProduction
} // namespace nsApp

using EndRollText = nsApp::nsProduction::EndRollText;
