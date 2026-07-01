#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/title/TitleView.h"
#include "Src/UI/PatchNote/PatchNoteScreen.h"
#include "Src/UI/Select/ManualUI.h"
#include "Src/UI/Select/TitleMenu.h"
#include "system/system.h"

namespace nsApp
{
    namespace nsScene
    {
        TitleScene::~TitleScene()
        {
            if (pTitleView_)
                DeleteGO(pTitleView_);

            if (pTitleMenu_)
                DeleteGO(pTitleMenu_);

            if (pManualUI_)
                DeleteGO(pManualUI_);

            if (pPatchNoteScreen_)
                DeleteGO(pPatchNoteScreen_);
        }

        bool TitleScene::Start()
        {
            pTitleView_ = NewGO<nsTitle::TitleView>(0, "titleview");
            if (pTitleView_ == nullptr)
                return false;

            pTitleMenu_ = NewGO<nsUI::TitleMenu>(1, "titleMenu");
            pTitleMenu_->Init();
            pTitleMenu_->Open();

            pManualUI_ = NewGO<nsUI::ManualUI>(1, "ManualUI");
            pManualUI_->Init();

            pPatchNoteScreen_ = NewGO<nsUI::PatchNoteScreen>(1, "PatchNoteScreen");
            pPatchNoteScreen_->Init();

            nsCore::SoundManager::GetInstance()->ReleaseAllSounds();
            nsCore::SoundManager::GetInstance()->PlayBGM(GameSoundList_BGM_Title);

            pFade_ = SceneManager::GetInstance()->GetFade();
            SceneManager::GetInstance()->SetEnableAutoLoading(false);

            isFadingOut = false;
            titleState_ = TitleState::Normal;

            pFade_->StartFadeIn();
            return true;
        }

        void TitleScene::Update()
        {
            if (pFade_ == nullptr)
                return;

            if (isGameEndRequested_)
            {
                g_gameLoop_.isLoop_ = false;
                return;
            }

            UpdateTitleState();
        }

        void TitleScene::WaitInputAction()
        {
            isUp_ = g_pad[0]->IsTrigger(enButtonUp);
            isDown_ = g_pad[0]->IsTrigger(enButtonDown);
            isDecide_ = g_pad[0]->IsTrigger(enButtonA);

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
            case TitleState::FadingToPatchNote:
                UpdateFadingToPatchNoteState();
                break;
            case TitleState::PatchNoteOpen:
                UpdatePatchNoteOpenState();
                break;
            case TitleState::FadingToMenuFromPatchNote:
                UpdateFadingToMenuFromPatchNoteState();
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
            if (!pFade_->IsFadeOutEnd())
            {
                //! Start / Select でパッチノートを開く
                if (g_pad[0]->IsTrigger(enButtonStart) || g_pad[0]->IsTrigger(enButtonSelect))
                {
                    pFade_->StartFadeOut();
                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button);
                    titleState_ = TitleState::FadingToPatchNote;
                    return;
                }

                WaitInputAction();
            }
        }

        void TitleScene::UpdateFadingToManualState()
        {
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
            if (g_pad[0]->IsTrigger(enButtonB))
            {
                pFade_->StartFadeOut();
                nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SelectScreen_Cancel);
                titleState_ = TitleState::FadingToMenu;
            }
        }

        void TitleScene::UpdateFadingToMenuState()
        {
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

        void TitleScene::UpdateFadingToPatchNoteState()
        {
            if (pFade_->IsFadeOutEnd())
            {
                if (pTitleMenu_)
                    pTitleMenu_->Close();

                if (pPatchNoteScreen_)
                    pPatchNoteScreen_->Open();

                if (pTitleView_)
                {
                    pTitleView_->SetShowLogo(false);
                    pTitleView_->SetManualMode(true);
                    pTitleView_->SetShowInformation(false);
                }

                pFade_->StartFadeIn();
                titleState_ = TitleState::PatchNoteOpen;
            }
        }

        void TitleScene::UpdatePatchNoteOpenState()
        {
            const bool isUp = g_pad[0]->IsTrigger(enButtonUp);
            const bool isDown = g_pad[0]->IsTrigger(enButtonDown);

            if (pPatchNoteScreen_ && pPatchNoteScreen_->UpdateInput(isUp, isDown))
            {
                pFade_->StartFadeOut();
                nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SelectScreen_Cancel);
                titleState_ = TitleState::FadingToMenuFromPatchNote;
            }
        }

        void TitleScene::UpdateFadingToMenuFromPatchNoteState()
        {
            if (pFade_->IsFadeOutEnd())
            {
                if (pPatchNoteScreen_)
                    pPatchNoteScreen_->Close();

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

                    if (pPatchNoteScreen_)
                    {
                        DeleteGO(pPatchNoteScreen_);
                        pPatchNoteScreen_ = nullptr;
                    }

                    nsK2EngineLow::GameObjectManager::GetInstance()->FlushDeadGameObjects();
                    SceneManager::GetInstance()->ChangeScene((nsScene::SceneID) nextSceneID_);
                }
            }
        }


        void TitleScene::UpdateGameEndFadeState()
        {
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

                if (pPatchNoteScreen_)
                {
                    DeleteGO(pPatchNoteScreen_);
                    pPatchNoteScreen_ = nullptr;
                }

                isGameEndRequested_ = true;
            }
        }
    } // namespace nsScene
} // namespace nsApp
