#include "stdafx.h"

#include "SoundManager.h"

namespace nsApp
{
    namespace nsCore
    {
        SoundManager* SoundManager::pInstance_ = nullptr;


        float SoundManager::Clamp01(float v)
        {
            /* ボリュームを0〜1ノア範囲に制限する。*/
            if (v < 0.0f)
                return 0.0f;
            if (v > 1.0f)
                return 1.0f;
            return v;
        }


        bool SoundManager::IsBgm(GameSoundList id)
        {
            return id >= GameSoundList_BGM_Title && id <= GameSoudList_BGM_EndRoll;
        }


        float SoundManager::EffectiveBgm(float localVol) const
        {
            if (masterMuted_ || bgmMuted_)
                return 0.0f;

            return Clamp01(localVol) * masterVolume_ * bgmVolume_;
        }


        float SoundManager::EffectiveSe(float localVol) const
        {
            if (masterMuted_ || seMuted_)
                return 0.0f;

            return Clamp01(localVol) * masterVolume_ * seVolume_;
        }


        void SoundManager::DeleteInstence()
        {
            if (pInstance_ != nullptr)
            {
                if (pInstance_->pSoundEngine_ != nullptr)
                {
                    pInstance_->pSoundEngine_->ReleaseAllSounds();
                    delete pInstance_->pSoundEngine_;
                    pInstance_->pSoundEngine_ = nullptr;
                }
                delete pInstance_;
                pInstance_ = nullptr;
            }
        }


        SoundManager* SoundManager::GetInstance()
        {
            if (pInstance_ == nullptr)
            {
                CreateInstence();
                pInstance_->Init();
            }
            return pInstance_;
        }


        void SoundManager::Init()
        {
            if (pSoundEngine_ != nullptr)
                return;

            pSoundEngine_ = new GameSoundEngine();
            pSoundEngine_->Init();
        }


        void SoundManager::PlayBGM(GameSoundList bgm, float vol)
        {
            if (pSoundEngine_ == nullptr)
                return;

            currentBgm_ = bgm;
            pSoundEngine_->PlayBGM(bgm, EffectiveBgm(vol));
        }


        void SoundManager::PlaySE(GameSoundList se, float vol)
        {
            if (pSoundEngine_ == nullptr)
                return;

            pSoundEngine_->PlaySE(se, EffectiveSe(vol));
        }


        void SoundManager::SetMasterVolume(float volume)
        {
            masterVolume_ = Clamp01(volume);
            ApplyCategoryVolumes();
        }


        void SoundManager::SetBgmVolume(float volume)
        {
            bgmVolume_ = Clamp01(volume);
            ApplyCategoryVolumes();
        }


        void SoundManager::SetSeVolume(float volume)
        {
            seVolume_ = Clamp01(volume);
            ApplyCategoryVolumes();
        }


        void SoundManager::SetMasterMuted(bool muted)
        {
            masterMuted_ = muted;
            ApplyCategoryVolumes();
        }


        void SoundManager::SetBgmMuted(bool muted)
        {
            bgmMuted_ = muted;
            ApplyCategoryVolumes();
        }


        void SoundManager::SetSeMuted(bool muted)
        {
            seMuted_ = muted;
            ApplyCategoryVolumes();
        }


        void SoundManager::ApplyCategoryVolumes()
        {
            if (pSoundEngine_ == nullptr)
                return;

            /* 再生中 BGM にカテゴリ音量を再適用する。 */
            if (currentBgm_ != GameSoundList_Num && pSoundEngine_->IsPlayingSound(currentBgm_))
                pSoundEngine_->SetVolume(currentBgm_, EffectiveBgm(1.0f));
        }
    } // namespace nsCore
} // namespace nsApp
