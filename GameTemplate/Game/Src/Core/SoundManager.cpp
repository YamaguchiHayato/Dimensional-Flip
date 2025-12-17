#include "stdafx.h"
#include "SoundManager.h"

namespace app
{
    namespace core
    {
        SoundManager* SoundManager::pInstance_ = nullptr;


        void SoundManager::DeleteInstence()
        {
            if (pInstance_ != nullptr)
            {
                delete pInstance_->pSoundEngine_;
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
    }
}
