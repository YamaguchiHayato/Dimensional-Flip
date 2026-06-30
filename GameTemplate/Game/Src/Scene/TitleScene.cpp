#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/title/TitleView.h"
#include "Src/UI/Select/ManualUI.h"
#include "Src/UI/Select/TitleMenu.h"
#include "system/system.h"

namespace nsApp
{
    namespace nsScene
    {
        TitleScene::~TitleScene()
        {
            /**
             * @brief タイトル描画用 GO を削除する
             */
            if (pTitleView_)
                DeleteGO(pTitleView_);

            /**
             * @brief メニュー GO を削除する
             */
            if (pTitleMenu_)
                DeleteGO(pTitleMenu_);

            /**
             * @brief マニュアル UI GO を削除する
             */
            if (pManualUI_)
                DeleteGO(pManualUI_);
        }

        bool TitleScene::Start()
        {
            /**
             * @brief 背景・ロゴ・Ver 表示を束ねる TitleView を生成する
             */
            pTitleView_ = NewGO<nsTitle::TitleView>(0, "titleview");
            if (pTitleView_ == nullptr)
                return false;

            /**
             * @brief メニュー UI を生成して表示する
             */
            pTitleMenu_ = NewGO<nsUI::TitleMenu>(1, "titleMenu");
            pTitleMenu_->Init();
            pTitleMenu_->Open();

            /**
             * @brief マニュアル UI を生成する（初期は非表示）
             */
            pManualUI_ = NewGO<nsUI::ManualUI>(1, "ManualUI");
            pManualUI_->Init();

            /**
             * @brief タイトル BGM を再生する
             */
            nsCore::SoundManager::GetInstance()->ReleaseAllSounds();
            nsCore::SoundManager::GetInstance()->PlayBGM(GameSoundList_BGM_Title);

            pFade_ = SceneManager::GetInstance()->GetFade();

            SceneManager::GetInstance()->SetEnableAutoLoading(false);

            isFadingOut = false;
            titleState_ = TitleState::Normal;

            /**
             * @brief シーン入場と同時にフェードインを開始する
             */
            pFade_->StartFadeIn();

            return true;
        }

        void TitleScene::Update()
        {
            if (pFade_ == nullptr)
                return;

            /**
             * @brief ゲーム終了が選ばれたらメインループを止める
             */
            if (isGameEndRequested_)
            {
                g_gameLoop_.isLoop_ = false;
                return;
            }

            UpdateTitleState();
        }

        void TitleScene::WaitInputAction()
        {
            isUp_ = g_pad[0]->IsTrigger(enButtonUp);     //! 上入力
            isDown_ = g_pad[0]->IsTrigger(enButtonDown); //! 下入力
            isDecide_ = g_pad[0]->IsTrigger(enButtonA);  //! 決定入力

            /**
             * @brief メニュー更新。決定されたら選択項目に応じて遷移する
             */
            if (pTitleMenu_ && pTitleMenu_->Update(isUp_, isDown_, isDecide_))
            {
                selectType_ = pTitleMenu_->GetCurrentSelect();

                switch (selectType_)
                {
                case nsUI::TitleMenuType::GameStart:
                    nextSceneID_ = (int) nsScene::SceneID::sWorldSelect;
                    SceneManager::GetInstance()->SetEnableAutoLoading(true);
                    pFade_->StartFadeOut();
                    titleState_ = TitleState::GameStartFade;
                    break;

                case nsUI::TitleMenuType::Manual:
                    pFade_->StartFadeOut();
                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button);
                    titleState_ = TitleState::FadingToManual;
                    break;

                case nsUI::TitleMenuType::GameEnd:
                    pFade_->StartFadeOut();
                    titleState_ = TitleState::GameEndFade;
                    break;
                }
            }
        }

        void TitleScene::UpdateTitleState()
        {
            switch (titleState_)
            {
            case TitleState::Normal:
                UpdateNormalState();
                break;
            case TitleState::FadingToManual:
                UpdateFadingToManualState();
                break;
            case TitleState::ManualOpen:
                UpdateManualOpenState();
                break;
            case TitleState::FadingToMenu:
                UpdateFadingToMenuState();
                break;
            case TitleState::GameStartFade:
                UpdateGameStartFadeState();
                break;
            case TitleState::GameEndFade:
                UpdateGameEndFadeState();
                break;
            default:
                break;
            }
        }

        void TitleScene::UpdateNormalState()
        {
            /**
             * @brief フェード中でなければメニュー入力を受け付ける
             */
            if (!pFade_->IsFadeOutEnd())
                WaitInputAction();
        }

        void TitleScene::UpdateFadingToManualState()
        {
            /**
             * @brief 暗転完了後にマニュアル UI へ切り替える
             */
            if (pFade_->IsFadeOutEnd())
            {
                if (pTitleMenu_)
                    pTitleMenu_->Close();

                if (pManualUI_)
                    pManualUI_->Open();

                if (pTitleView_)
                {
                    pTitleView_->SetShowLogo(false);
                    pTitleView_->SetManualMode(true);
                    pTitleView_->SetShowInformation(false);
                }

                pFade_->StartFadeIn();
                titleState_ = TitleState::ManualOpen;
            }
        }

        void TitleScene::UpdateManualOpenState()
        {
            /**
             * @brief B ボタンでマニュアルを閉じてタイトルへ戻る
             */
            if (g_pad[0]->IsTrigger(enButtonB))
            {
                pFade_->StartFadeOut();
                nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SelectScreen_Cancel);
                titleState_ = TitleState::FadingToMenu;
            }
        }

        void TitleScene::UpdateFadingToMenuState()
        {
            /**
             * @brief 暗転完了後にメニュー表示へ戻す
             */
            if (pFade_->IsFadeOutEnd())
            {
                if (pManualUI_)
                    pManualUI_->Close();

                if (pTitleMenu_)
                    pTitleMenu_->Open();

                if (pTitleView_)
                {
                    pTitleView_->SetShowLogo(true);
                    pTitleView_->SetManualMode(false);
                    pTitleView_->SetShowInformation(false);
                }

                pFade_->StartFadeIn();
                titleState_ = TitleState::Normal;
            }
        }

        void TitleScene::UpdateGameStartFadeState()
        {
            /**
             * @brief フェードアウト完了後にタイトル GO を破棄してからワールド Select へ遷移する
             */
            if (pFade_->IsFadeOutEnd())
            {
                if (nextSceneID_ != -1)
                {
                    if (pTitleView_)
                    {
                        DeleteGO(pTitleView_);
                        pTitleView_ = nullptr;
                    }

                    if (pTitleMenu_)
                    {
                        DeleteGO(pTitleMenu_);
                        pTitleMenu_ = nullptr;
                    }

                    if (pManualUI_)
                    {
                        DeleteGO(pManualUI_);
                        pManualUI_ = nullptr;
                    }

                    nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects();

                    SceneManager::GetInstance()->ChangeScene((nsScene::SceneID) nextSceneID_);
                }
            }
        }

        void TitleScene::UpdateGameEndFadeState()
        {
            /**
             * @brief フェード完了後に GO を破棄して終了フラグを立てる
             */
            if (pFade_->IsFadeOutEnd())
            {
                if (pTitleView_)
                {
                    DeleteGO(pTitleView_);
                    pTitleView_ = nullptr;
                }

                if (pTitleMenu_)
                {
                    DeleteGO(pTitleMenu_);
                    pTitleMenu_ = nullptr;
                }

                if (pManualUI_)
                {
                    DeleteGO(pManualUI_);
                    pManualUI_ = nullptr;
                }

                isGameEndRequested_ = true;
            }
        }
    } // namespace nsScene
} // namespace nsApp
