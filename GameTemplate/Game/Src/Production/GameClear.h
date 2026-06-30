#pragma once

#include "ResultData.h"

/**
 * @file   GameClear.h
 * @brief  ステージクリア後のリザルト演出 GO。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @enum GameClearState
         * @brief リザルト画面の演出段階。
         */
        enum class GameClearState : uint8_t
        {
            FadeIn,     //!< 最初のフェード
            ShowLogo,   //!< RESULT ロゴ表示
            CountTime,  //!< タイムのカウント演出
            CountScore, //!< スコアのカウント演出
            WaitInput,  //!< 入力待ち
        };

        /**
         * @class GameClear
         * @brief  クリアタイム・スコアのカウント演出と入力待ち UI。
         */
        class GameClear : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            GameClear() = default;
            virtual ~GameClear() = default;

        public:
            /**
             * @brief 初期化。各 UI フォント・スプライトを構築する。
             * @return 成功時 true。
             */
            bool Start();

            /**
             * @brief 演出ステートを更新する。
             */
            void Update();

            /**
             * @brief リザルト UI を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);

        public:
            /**
             * @brief クリアデータを受け取る。
             * @param data StageManager が保存したリザルトデータ。
             */
            inline void SetUpResultData(const nsStage::StageResultData& data)
            {
                resultData_ = data;
            }

            /**
             * @brief 演出が全て終了したか。
             * @return WaitInput なら true。
             */
            inline bool IsFinished() const
            {
                return state_ == GameClearState::WaitInput;
            }

        private:
            void InitResultFrame();     //!< リザルトフレーム UI を初期化する。
            void InitGameClearFont();   //!< 「Game Clear」表記を初期化する。
            void InitTimerFont();       //!< タイム表示を初期化する。
            void InitScoreFont();       //!< スコア表示を初期化する。
            void CleanUpUI();           //!< 不要な UI GO を削除する。
            void InitInputPromptFont(); //!< 入力促進テキストを初期化する。


        private:
            SpriteRender resultFrameSprite_; //!< リザルトフレーム画像
            FontRender resultFont_;          //!< 「Game Clear」表記
            FontRender timerFont_;           //!< タイム表示
            FontRender scoreFont_;           //!< スコア表示
            FontRender inputPromptFont_;     //!< 「タイトルに戻る」表記

            GameClearState state_ = GameClearState::ShowLogo; //!< 現在の演出段階
            nsStage::StageResultData resultData_;               //!< 受け取ったクリアデータ

            float targetTime_ = 0.0f;   //!< 目標タイム（予約）
            float disPlayTime = 0.0f;   //!< 表示中タイム
            float finishTimer_ = 0.0f;  //!< 点滅用タイマー

            int targetScore_ = 0;   //!< 目標スコア（予約）
            int displayScore_ = 0;    //!< 表示中スコア
            int timer_ = 0;           //!< 汎用フレームカウンター
        };
    } // namespace nsProduction
} // namespace nsApp

using GameClear      = nsApp::nsProduction::GameClear;
using GameClearState = nsApp::nsProduction::GameClearState;
