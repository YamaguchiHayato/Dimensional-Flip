#include "stdafx.h"

#include "SoundSettingController.h"
#include "Src/Core/Game.h"
#include "Src/Presentation/Data/SoundSettingData.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreen.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreenHost.h"
#include "Src/UI/Pause/PauseMenuUI.h"

namespace nsApp
{
    namespace nsCore
    {
        void SoundSettingController::Initialize(nsUI::PauseMenuUI* pPauseMenu, nsUI::SoundSettingScreen* pScreen,
                                                nsPresentation::SoundSettingData* pData)
        {
            pPauseMenu_ = pPauseMenu;
            pScreen_ = pScreen;
            pData_ = pData;
            isOpen_ = false;
            selectedIndex_ = 0;
        }

        void SoundSettingController::EnsureScreen()
        {
            if (pScreen_ != nullptr)
                return;

            if (auto* pHost = FindGO<nsUI::SoundSettingScreenHost>("SoundSettingScreenHost"))
            {
                if (!pHost->IsStart())
                    pHost->StartWrapper();
                pScreen_ = pHost->GetSoundSettingScreen();
            }
        }

        void SoundSettingController::Open()
        {
            EnsureScreen();
            if (isOpen_)
                return;

            isOpen_ = true;
            selectedIndex_ = 0;

            if (pPauseMenu_)
                pPauseMenu_->Close();
            if (pScreen_)
            {
                pScreen_->SetVisible(true);
                /* 開き始めは Master 行を選択状態にする。 */
                pScreen_->SetSelectedIndex(0);
            }

            g_gameTime->EnableFixedFrameDeltaTime(0.0f);
        }

        void SoundSettingController::CloseToPause()
        {
            if (!isOpen_)
                return;

            isOpen_ = false;
            if (pScreen_)
                pScreen_->SetVisible(false);
            if (pPauseMenu_)
                pPauseMenu_->Open();

            g_gameTime->EnableFixedFrameDeltaTime(0.0f);
        }


        void SoundSettingController::ApplySelectionInput()
        {
            /* 変更前の選択を覚えておく。 */
            const int prev = selectedIndex_;

            if (g_pad[0]->IsTrigger(enButtonUp))
                selectedIndex_ = (selectedIndex_ - 1 + kRowCount) % kRowCount;

            if (g_pad[0]->IsTrigger(enButtonDown))
                selectedIndex_ = (selectedIndex_ + 1) % kRowCount;

            /* 選択が変わったら Screen へ伝えて見た目を更新する。 */
            if (prev != selectedIndex_ && pScreen_ != nullptr)
                pScreen_->SetSelectedIndex(selectedIndex_);
        }


        void SoundSettingController::ApplyValueInput()
        {
            if (pData_ == nullptr)
                return;

            /* 左右で音量（または Controller ON/OFF）。 */
            const bool left = g_pad[0]->IsTrigger(enButtonLeft);
            const bool right = g_pad[0]->IsTrigger(enButtonRight);
            const bool decide = g_pad[0]->IsTrigger(enButtonA);

            if (selectedIndex_ == 3)
            {
                /* Controller 行。 */
                if (left)
                    pData_->SetControllerEnabled(false);
                if (right)
                    pData_->SetControllerEnabled(true);
                if (decide)
                    pData_->SetControllerEnabled(!pData_->IsControllerEnabled());
                return;
            }

            /* 音量行: 左右でノブ移動、A でミュート切替。 */
            auto getVol = [&]() -> float
            {
                if (selectedIndex_ == 0)
                    return pData_->GetMasterVolume();
                if (selectedIndex_ == 1)
                    return pData_->GetBgmVolume();
                return pData_->GetSeVolume();
            };
            auto setVol = [&](float v)
            {
                if (selectedIndex_ == 0)
                    pData_->SetMasterVolume(v);
                else if (selectedIndex_ == 1)
                    pData_->SetBgmVolume(v);
                else
                    pData_->SetSeVolume(v);
            };
            auto toggleMute = [&]()
            {
                if (selectedIndex_ == 0)
                    pData_->SetMasterMuted(!pData_->IsMasterMuted());
                else if (selectedIndex_ == 1)
                    pData_->SetBgmMuted(!pData_->IsBgmMuted());
                else
                    pData_->SetSeMuted(!pData_->IsSeMuted());
            };

            if (left)
                setVol(getVol() - kVolumeStep);
            if (right)
                setVol(getVol() + kVolumeStep);
            if (decide)
                toggleMute();
        }


        SoundSettingAction SoundSettingController::Update(Game* pGame)
        {
            if (!isOpen_)
                return SoundSettingAction::None;

            /* Game から Data を補完する。 */
            if (pData_ == nullptr && pGame != nullptr)
                pData_ = pGame->GetSoundSettingData();

            if (g_pad[0]->IsTrigger(enButtonB))
            {
                CloseToPause();
                return SoundSettingAction::BackToPause;
            }

            ApplySelectionInput();
            ApplyValueInput();
            return SoundSettingAction::None;
        }
    } // namespace nsCore
} // namespace nsApp
