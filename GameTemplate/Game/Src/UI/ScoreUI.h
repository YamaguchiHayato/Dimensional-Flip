#pragma once

#include "Src/Actor/Character/PlayerForward.h"
#include "Src/UI/UIBase.h"

/**
 * @file   ScoreUI.h
 * @brief  画面上部のスコア表示 UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class ScoreUI
         * @brief スコア数値をドラムロール表示するシングルトン GO。
         */
        class ScoreUI : public UIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            ScoreUI() = default;
            virtual ~ScoreUI() = default;


        public:
            /**
             * @brief スコア表示用フォントを初期化する。
             * @return 初期化が成功したら true、失敗したら false。
             */
            bool Start() override;

            /**
             * @brief UIBase::Start をオーバーライドしてプレイヤー参照を取得する。
             */
            void Update() override;

            /**
             * @brief スコア表示を更新する。ドラムロール演出を行う。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override;

            /** @brief スコア表示をプレイヤー値に同期する。 */
            void UpdateScore();

            /** @brief スコアを徐々に追従させる（ドラムロール）。 */
            void UpdateScoreScroll();

            /** @brief 表示スコアを 0 にリセットする。 */
            inline void ResetScore() { score_ = 0.0f; }

            /** @return 現在の表示スコア（整数換算）。 */
            inline float GetScore() const { return static_cast<int>(score_); }

            /** @return シングルトンインスタンス。 */
            inline static ScoreUI* GetInstance() { return instance_; }


        private:
            /**
             * @brief シングルトンインスタンスを破棄する。
             */
            void InitUIScore();


        private:
            static ScoreUI* instance_; //!< シングルトン。

            Player* pPlayer_ = nullptr; //!< スコア参照元。

            FontRender scoreFont_;         //!< 数値（000000）。
            FontRender scoreFontUIRender_; //!< "SCORE" ラベル。

            float score_ = 0.0f;    //!< 表示中スコア（補間用）。
            wchar_t scoreText_[64]; //!< フォント用バッファ。
        };
    } // namespace nsUI
} // namespace nsApp

using ScoreUI = nsApp::nsUI::ScoreUI;

namespace app
{
    namespace nsUI
    {
        using ScoreUI = nsApp::nsUI::ScoreUI;
    } // namespace nsUI
} // namespace app
