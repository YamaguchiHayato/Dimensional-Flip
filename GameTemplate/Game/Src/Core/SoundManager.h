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
         * @brief GameSoundEngine への薄いラッパー。カテゴリ音量を保持する。
         */
        class SoundManager
        {
        public:
            /**
             * @brief シングルトンを生成する。CreateInstence() 後に GetInstance() で取得する。
             */
            inline static void CreateInstence()
            {
                if (pInstance_ == nullptr)
                    pInstance_ = new SoundManager();
            }

            /**
             * @brief シングルトンを破棄する。DeleteInstence() 後に GetInstance() は nullptr を返す。
             */
            static void DeleteInstence();

            /**
             * @brief シングルトンを取得する。CreateInstence() 後に呼ぶこと。
             * @return SoundManager シングルトン。CreateInstence() 前は nullptr。
             */
            static SoundManager* GetInstance();

            /**
             * @brief GameSoundEngine を生成し、カテゴリ音量を初期化する。
             */
            void Init();

            /**
             * @brief BGM を再生する（カテゴリ音量を掛ける）。
             */
            void PlayBGM(GameSoundList bgm, float vol = 1.0f);

            /**
             * @brief SE を再生する（カテゴリ音量を掛ける）。
             */
            void PlaySE(GameSoundList se, float vol = 1.0f);

            /**
             * @brief 再生中の BGM を停止する。
             * @param sound 停止する BGM ID。再生中でなければ何もしない。
             */
            inline void StopBGM(GameSoundList sound)
            {
                if (pSoundEngine_)
                    pSoundEngine_->StopSound(sound);
            }

            /**
             * @brief 再生中の SE を停止する。
             * @param se 停止する SE ID。再生中でなければ何もしない。
             */
            inline void StopSE(GameSoundList se)
            {
                if (pSoundEngine_)
                    pSoundEngine_->StopSound(se);
            }

            /**
             * @brief 再生中のサウンドの音量を変更する。
             * @param sound 変更するサウンド ID。再生中でなければ何もしない。
             * @param vol 変更後の音量（0.0f～1.0f）。
             */
            inline void SetVolume(GameSoundList sound, float vol)
            {
                if (pSoundEngine_)
                    pSoundEngine_->SetVolume(sound, vol);
            }

            /**
             * @brief カテゴリ音量を設定する。
             * @param volume 0.0f～1.0f の範囲で設定する。範囲外はクランプされる。
             */
            void SetMasterVolume(float volume);

            /**
             * @brief BGM カテゴリ音量を設定する。
             * @param volume 0.0f～1.0f の範囲で設定する。範囲外はクランプされる。
             */
            void SetBgmVolume(float volume);

            /**
             * @brief SE カテゴリ音量を設定する。
             * @param volume 0.0f～1.0f の範囲で設定する。範囲外はクランプされる。
             */
            void SetSeVolume(float volume);

            /**
             * @brief カテゴリのミュート状態を設定する。
             * @param muted true でミュート、false でミュート解除。
             */
            void SetMasterMuted(bool muted);

            /**
             * @brief BGM カテゴリのミュート状態を設定する。
             * @param muted true でミュート、false でミュート解除。
             */
            void SetBgmMuted(bool muted);

            /**
             * @brief SE カテゴリのミュート状態を設定する。
             * @param muted true でミュート、false でミュート解除。
             */
            void SetSeMuted(bool muted);

            /**
             * @brief 再生中 BGM へ現在のカテゴリ音量を再適用する。
             */
            void ApplyCategoryVolumes();

            /**
             * @brief 毎フレーム更新する。GameSoundEngine::Update() を呼ぶ。
             */
            void Update()
            {
                if (pSoundEngine_)
                    pSoundEngine_->Update();
            }

            /**
             * @brief 再生中の全サウンドを停止する。
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
            /**
             * @brief 0.0f～1.0f の範囲にクランプする。
             * @param v クランプする値。
             * @return 0.0f～1.0f の範囲にクランプされた値。
             */
            static float Clamp01(float v);

            /**
             * @brief カテゴリ音量とミュート状態を掛け合わせた実効音量を計算する。
             * @param localVol ローカル音量（0.0f～1.0f）。
             * @return カテゴリ音量とミュート状態を掛け合わせた実効音量（0.0f～1.0f）。
             */
            float EffectiveBgm(float localVol) const;

            /**
             * @brief カテゴリ音量とミュート状態を掛け合わせた実効音量を計算する。
             * @param localVol ローカル音量（0.0f～1.0f）。
             * @return カテゴリ音量とミュート状態を掛け合わせた実効音量（0.0f～1.0f）。
             */
            float EffectiveSe(float localVol) const;

            /**
             * @brief 指定されたサウンド ID が BGM かどうかを判定する。
             * @param id 判定するサウンド ID。
             * @return BGM なら true、SE なら false。
             */
            static bool IsBgm(GameSoundList id);


        private:
            GameSoundEngine* pSoundEngine_ = nullptr; //! < GameSoundEngine インスタンス。nullptr 可。
            static SoundManager* pInstance_;//! < SoundManager シングルトン。CreateInstence() 後に GetInstance() で取得する。

            float masterVolume_ = 1.0f; //! < カテゴリ音量。0.0f～1.0f の範囲で設定する。
            float bgmVolume_ = 1.0f;    //! < BGM カテゴリ音量。0.0f～1.0f の範囲で設定する。
            float seVolume_ = 1.0f;     //! < SE カテゴリ音量。0.0f～1.0f の範囲で設定する。
            bool masterMuted_ = false;  //! < カテゴリミュート状態。true でミュート、false でミュート解除。
            bool bgmMuted_ = false;     //! < BGM カテゴリミュート状態。true でミュート、false でミュート解除。
            bool seMuted_ = false;      //! < SE カテゴリミュート状態。true でミュート、false でミュート解除。
            GameSoundList currentBgm_ = GameSoundList_Num;//! < 現在再生中の BGM ID。再生中でなければ GameSoundList_Num。
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
