#include "stdafx.h"

#include "SoundSettingData.h"
#include "Src/Core/SoundManager.h"
#include "Src/Presentation/UI/Screens/SoundSettingScreen.h"

namespace nsApp
{
    namespace nsPresentation
    {
        float SoundSettingData::Clamp01(float value)
        {
            if (value < 0.0f)
                return 0.0f;
            if (value > 1.0f)
                return 1.0f;
            return value;
        }


        void SoundSettingData::SetScreen(nsUI::SoundSettingScreen* pScreen)
        {
            /* Screen を登録し、即時同期する。 */
            pScreen_ = pScreen;
            SyncToScreen();
        }


        void SoundSettingData::SetMasterVolume(float volume)
        {
            masterVolume_ = Clamp01(volume);

            /* 実音源（Master カテゴリ）へ反映する。 */
            if (auto* sm = nsCore::SoundManager::GetInstance())
                sm->SetMasterVolume(masterVolume_);

            SyncToScreen();
        }


        void SoundSettingData::SetBgmVolume(float volume)
        {
            bgmVolume_ = Clamp01(volume);

            /* 実音源（BGM カテゴリ）へ反映する。 */
            if (auto* sm = nsCore::SoundManager::GetInstance())
                sm->SetBgmVolume(bgmVolume_);

            SyncToScreen();
        }


        void SoundSettingData::SetSeVolume(float volume)
        {
            seVolume_ = Clamp01(volume);

            /* 実音源（SE カテゴリ）へ反映する。 */
            if (auto* sm = nsCore::SoundManager::GetInstance())
                sm->SetSeVolume(seVolume_);

            SyncToScreen();
        }


        void SoundSettingData::SetMasterMuted(bool muted)
        {
            isMasterMuted_ = muted;

            if (auto* sm = nsCore::SoundManager::GetInstance())
                sm->SetMasterMuted(isMasterMuted_);

            SyncToScreen();
        }


        void SoundSettingData::SetBgmMuted(bool muted)
        {
            isBgmMuted_ = muted;

            if (auto* sm = nsCore::SoundManager::GetInstance())
                sm->SetBgmMuted(isBgmMuted_);

            SyncToScreen();
        }


        void SoundSettingData::SetSeMuted(bool muted)
        {
            isSeMuted_ = muted;

            if (auto* sm = nsCore::SoundManager::GetInstance())
                sm->SetSeMuted(isSeMuted_);

            SyncToScreen();
        }


        void SoundSettingData::SetControllerEnabled(bool enabled)
        {
            /* Controller は見た目・設定保持のみ（振動は後続対応可）。 */
            isControllerEnabled_ = enabled;
            SyncToScreen();
        }


        void SoundSettingData::SyncToScreen()
        {
            /* Screen へ委譲する。 */
            if (pScreen_ != nullptr)
                pScreen_->SyncFromDataSource();
        }
    } // namespace nsPresentation
} // namespace nsApp
