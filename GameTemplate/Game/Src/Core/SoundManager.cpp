#include "stdafx.h"

#include "SoundManager.h"

namespace nsApp
{
    namespace nsCore
    {
        SoundManager* SoundManager::pInstance_ = nullptr;

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
    } // namespace nsCore
} // namespace nsApp
