#pragma once

#include "GameSoundEngine.h"

/**
 * @file   SoundManager.h
 * @brief  BGM / SE の再生を統括するシングルトン。
 */

namespace nsApp
{
    namespace nsCore
    {
        /**
         * @class SoundManager
         * @brief GameSoundEngine への薄いラッパー。
         */
        class SoundManager
        {
        public:
            /**
             * @brief シングルトンのインスタンスを生成する。
             */
            inline static void CreateInstence()
            {
                if (pInstance_ == nullptr)
                    pInstance_ = new SoundManager();
            }

            /**
             * @brief シングルトンのインスタンスを破棄する。
             */
            static void DeleteInstence();

            /**
             * @brief シングルトンのインスタンスを取得する。
             */
            static SoundManager* GetInstance();

            /**
             * @brief GameSoundEngine を初期化する。
             */
            void Init();

            /**
             * @brief BGM を再生する。
             * @param bgm BGMの種類。
             * @param vol 音量（0.0f～1.0f）。
             */
            inline void PlayBGM(GameSoundList bgm, float vol = 1.0f)
            {
                if (pSoundEngine_)
                    pSoundEngine_->PlayBGM(bgm, vol);
            }

            /**
             * @brief SE を再生する。
             * @param se SEの種類。
             * @param vol 音量（0.0f～1.0f）。
             */
            inline void PlaySE(GameSoundList se, float vol = 1.0f)
            {
                if (pSoundEngine_)
                    pSoundEngine_->PlaySE(se, vol);
            }

            /**
             * @brief BGM を停止する。
             * @param sound 停止する BGM の種類。
             */
            inline void StopBGM(GameSoundList sound)
            {
                if (pSoundEngine_)
                    pSoundEngine_->StopSound(sound);
            }

            /**
             * @brief SE を停止する。
             * @param se 停止する SE の種類。
             */
            inline void StopSE(GameSoundList se)
            {
                if (pSoundEngine_)
                    pSoundEngine_->StopSound(se);
            }

            /**
             * @brief 音量を設定する。
             * @param sound 音量を設定するサウンドの種類。
             * @param vol 音量（0.0f～1.0f）。
             */
            inline void SetVolume(GameSoundList sound, float vol)
            {
                if (pSoundEngine_)
                    pSoundEngine_->SetVolume(sound, vol);
            }

            /**
             * @brief 毎フレームのサウンド更新処理。
             */
            void Update()
            {
                if (pSoundEngine_)
                    pSoundEngine_->Update();
            }

            /**
             * @brief すべてのサウンドを解放する。
             */
            void ReleaseAllSounds()
            {
                if (pSoundEngine_)
                    pSoundEngine_->ReleaseAllSounds();
            }

        private:
            /* コンストラクタとデストラクタ。*/
            SoundManager() = default;
            virtual ~SoundManager() = default;


        private:
            GameSoundEngine* pSoundEngine_ = nullptr; //! < GameSoundEngine のインスタンス。
            static SoundManager* pInstance_;          //! < SoundManager のシングルトンインスタンス。
        };
    } // namespace nsCore
} // namespace nsApp

namespace app
{
    namespace core
    {
        using SoundManager = nsApp::nsCore::SoundManager;
    }
} // namespace app
