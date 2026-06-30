#pragma once

#include "Src/Scene/Scene.h"
#include "Src/UI/Select/TitleMenu.h"
#include "Src/UI/Select/ManualUI.h"

/**
 * @file   TitleScene.h
 * @brief  タイトル画面シーン。
 */

namespace nsApp
{
    namespace nsProduction { class Fade; }
    namespace nsTitle      { class TitleView; }
    namespace nsUI         { class TitleMenu; class ManualUI; }

    namespace nsScene
    {
        /**
         * @enum TitleState
         * @brief タイトル画面の内部状態。
         */
        enum class TitleState : uint8_t
        {
            Normal,          //!< メニュー操作中
            FadingToManual,  //!< マニュアルへフェードアウト中
            ManualOpen,      //!< マニュアル表示中
            FadingToMenu,    //!< メニューへフェードイン中
            GameStartFade,   //!< ゲーム開始フェード中
            GameEndFade,     //!< 終了フェード中
        };

        /**
         * @class TitleScene
         * @brief タイトル表示・メニュー・マニュアル・シーン遷移を制御する。
         */
        class TitleScene : public IScene
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleScene() = default;
            virtual ~TitleScene();


        public:
            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 毎フレームのシーン更新（遷移判定・入力など）。
             */
            void Update() override;


        private:
            /**
             * @brief タイトル画面の入力待ち状態を更新する。
             */
            void WaitInputAction();

            /**
             * @brief タイトル画面の状態を更新する。
             */
            void UpdateTitleState();

            /**
             * @brief タイトル画面の状態を更新する（状態ごとの処理）。
             */
            void UpdateNormalState();

            /**
             * @brief タイトル画面の状態を更新する（状態ごとの処理）。
             */
            void UpdateFadingToManualState();

            /**
             * @brief タイトル画面の状態を更新する（状態ごとの処理）。
             */
            void UpdateManualOpenState();

            /**
             * @brief タイトル画面の状態を更新する（状態ごとの処理）。
             */
            void UpdateFadingToMenuState();

            /**
             * @brief タイトル画面の状態を更新する（状態ごとの処理）。
             */
            void UpdateGameStartFadeState();

            /**
             * @brief タイトル画面の状態を更新する（状態ごとの処理）。
             */
            void UpdateGameEndFadeState();


        private:
            nsProduction::Fade* pFade_ = nullptr; //! フェード用 GO。
            nsTitle::TitleView* pTitleView_ = nullptr; //! タイトルビュー用 GO。
            nsUI::TitleMenu* pTitleMenu_ = nullptr;    //! タイトルメニュー用 GO。
            nsUI::ManualUI* pManualUI_ = nullptr;      //! マニュアル UI 用 GO。

            bool isFadingOut = false; //!< フェードアウト中かどうか。
            bool isUp_ = false;       //! < 上方向入力中かどうか。
            bool isDown_ = false;     //!< 下方向入力中かどうか。
            bool isDecide_ = false;   //!< 決定入力中かどうか。
            bool isGameEndRequested_ = false; //! < ゲーム終了要求中かどうか。

            int nextSceneID_ = -1; //!< 次のシーン ID。（-1 は未設定）

            nsUI::TitleMenuType selectType_; //!< 選択中のメニュータイプ。
            TitleState titleState_ = TitleState::Normal; //!< タイトル画面の状態。
        };
    } // namespace nsScene
} // namespace nsApp

using TitleState = nsApp::nsScene::TitleState;
using TitleScene = nsApp::nsScene::TitleScene;
