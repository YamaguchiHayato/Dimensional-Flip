#pragma once

#include "Src/Scene/Scene.h"
#include "Src/UI/PatchNote/PatchNoteScreen.h"
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
    namespace nsUI         { class TitleMenu; class ManualUI; class PatchNoteScreen; }

    namespace nsScene
    {
        /**
         * @enum TitleState
         * @brief タイトル画面の内部状態。
         */
        enum class TitleState : uint8_t
        {
            Normal,                   //!< メニュー操作中
            FadingToManual,           //!< マニュアルへフェードアウト中
            ManualOpen,               //!< マニュアル表示中
            FadingToMenu,             //!< マニュアルからメニューへフェードイン中
            FadingToPatchNote,        //!< パッチノートへフェードアウト中
            PatchNoteOpen,            //!< パッチノート表示中
            FadingToMenuFromPatchNote,//!< パッチノートからメニューへフェードイン中
            GameStartFade,            //!< ゲーム開始フェード中
            GameEndFade,              //!< 終了フェード中
        };

        /**
         * @class TitleScene
         * @brief タイトル表示・メニュー・マニュアル・パッチノート・シーン遷移を制御する。
         */
        class TitleScene : public IScene
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleScene() = default;
            virtual ~TitleScene();

            /**
             * @brief タイトル画面シーンを開始する。
             * @return true: 成功, false: 失敗
             */
            bool Start() override;

            /**
             * @brief タイトル画面シーンを更新する。
             */
            void Update() override;


        private:
            /**
             * @brief タイトル画面の入力待機状態を更新する。
             */
            void WaitInputAction();

            /**  @brief タイトル画面の状態を更新する。*/
            void UpdateTitleState();
            void UpdateNormalState();
            void UpdateFadingToManualState();
            void UpdateManualOpenState();
            void UpdateFadingToMenuState();
            void UpdateFadingToPatchNoteState();
            void UpdatePatchNoteOpenState();
            void UpdateFadingToMenuFromPatchNoteState();
            void UpdateGameStartFadeState();
            void UpdateGameEndFadeState();


        private:
            nsProduction::Fade* pFade_ = nullptr; //! フェード制御クラス。
            nsTitle::TitleView* pTitleView_ = nullptr; //! タイトル画面のビュークラス。
            nsUI::TitleMenu* pTitleMenu_ = nullptr;    //! タイトルメニューUIクラス。
            nsUI::ManualUI* pManualUI_ = nullptr;      //! マニュアルUIクラス。
            nsUI::PatchNoteScreen* pPatchNoteScreen_ = nullptr; //! パッチノートUIクラス。

            bool isFadingOut = false; //! フェードアウト中かどうかのフラグ。
            bool isUp_ = false;       //!< 上方向の入力が押されているかどうかのフラグ。
            bool isDown_ = false;     //!< 下方向の入力が押されているかどうかのフラグ。
            bool isDecide_ = false;   //!< 決定ボタンが押されているかどうかのフラグ。
            bool isGameEndRequested_ = false; //! < ゲーム終了が要求されているかどうかのフラグ。

            int nextSceneID_ = -1; //! 次のシーンID。-1の場合は遷移しない。

            nsUI::TitleMenuType selectType_; //! ! 選択中のタイトルメニューの種類。
            TitleState titleState_ = TitleState::Normal; //!! タイトル画面の状態。
        };
    } // namespace nsScene
} // namespace nsApp

using TitleState = nsApp::nsScene::TitleState;
using TitleScene = nsApp::nsScene::TitleScene;
