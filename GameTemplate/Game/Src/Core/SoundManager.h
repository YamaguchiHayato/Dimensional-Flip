#pragma once
#include "GameSoundEngine.h"

namespace app
{
    namespace core
    {
        class SoundManager
        {
        public:
            inline static void CreateInstence()
            {
                if (pInstance_ == nullptr)
                    pInstance_ = new SoundManager();
            }


            static void DeleteInstence();


            static SoundManager* GetInstance();


           void Init();

            
           inline void PlayBGM(GameSoundList bgm, float vol = 1.0f)
            {
                if (pSoundEngine_)
                    pSoundEngine_->PlayBGM(bgm, vol);
            }

            inline void PlaySE(GameSoundList se, float vol = 1.0f)
            {
                if (pSoundEngine_)
                    pSoundEngine_->PlaySE(se, vol);
            }

            inline void StopBGM(GameSoundList sound)
            {
                if (pSoundEngine_)
                    pSoundEngine_->StopSound(sound);
            }

            inline void SetVolume(GameSoundList sound, float vol)
            {
                if (pSoundEngine_)
                    pSoundEngine_->SetVolume(sound, vol);
            }


        private:
            SoundManager() = default;
            virtual ~SoundManager() = default;

            GameSoundEngine* pSoundEngine_ = nullptr;

            static SoundManager* pInstance_;
        };

    }
}

