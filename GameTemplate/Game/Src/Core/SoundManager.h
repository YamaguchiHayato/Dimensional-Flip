#pragma once
#include "GameSoundEngine.h"

namespace app
{
    namespace core
    {
        class SoundManager
        {
        public:
            // シングルトンインスタンスの生成
            inline static void CreateInstence()
            {
                if (pInstance_ == nullptr)
                    pInstance_ = new SoundManager();
            }

            // シングルトンインスタンスの削除
            static void DeleteInstence();

            // シングルトンインスタンスの取得
            static SoundManager* GetInstance();

            // サウンドエンジンの初期化。
            void Init();

           // BGMの再生。
           inline void PlayBGM(GameSoundList bgm, float vol = 1.0f)
            {
                if (pSoundEngine_)
                    pSoundEngine_->PlayBGM(bgm, vol);
           }

           // SEの再生。
           inline void PlaySE(GameSoundList se, float vol = 1.0f)
           {
               if (pSoundEngine_)
                   pSoundEngine_->PlaySE(se, vol);
           }

           // BGMの停止
            inline void StopBGM(GameSoundList sound)
            {
                if (pSoundEngine_)
                    pSoundEngine_->StopSound(sound);
            }

            // SEの停止。
            inline void SetVolume(GameSoundList sound, float vol)
            {
                if (pSoundEngine_)
                    pSoundEngine_->SetVolume(sound, vol);
            }


        private:
            SoundManager() = default;
            virtual ~SoundManager() = default;


        private:
            GameSoundEngine* pSoundEngine_ = nullptr;

            static SoundManager* pInstance_;
        };

    }
}

