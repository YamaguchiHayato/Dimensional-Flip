#pragma once

#include "ISoundSettingData.h"

/**
 * @file   SoundSettingData.h
 * @brief  SoundSetting の保持データ（Screen との仲介）。
 */

namespace nsApp
{
    namespace nsUI
    {
        class SoundSettingScreen;
    }

    namespace nsPresentation
    {
        /**
         * @class SoundSettingData
         * @brief 音量・ミュート・Controller 状態を保持し Screen へ同期する。
         */
        class SoundSettingData : public ISoundSettingData
        {
        public:
            /**
             * @brief 反映先 Screen を登録する。
             * @param pScreen SoundSettingScreen。nullptr 可。
             */
            void SetScreen(nsUI::SoundSettingScreen* pScreen);

            /**
             * @brief Master 音量を設定する。
             * @param volume 0.0f〜1.0f。
             */
            void SetMasterVolume(float volume);

            /**
             * @brief BGM 音量を設定する。
             * @param volume 0.0f〜1.0f。
             */
            void SetBgmVolume(float volume);

            /**
             * @brief SE 音量を設定する。
             * @param volume 0.0f〜1.0f。
             */
            void SetSeVolume(float volume);

            /**
             * @brief Master ミュートを設定する。
             */
            void SetMasterMuted(bool muted);

            /**
             * @brief BGM ミュートを設定する。
             */
            void SetBgmMuted(bool muted);

            /**
             * @brief SE ミュートを設定する。
             */
            void SetSeMuted(bool muted);

            /**
             * @brief Controller ON/OFF を設定する。
             */
            void SetControllerEnabled(bool enabled);

            /**
             * @brief 保持データを Screen へ反映する。
             */
            void SyncToScreen();

            /**
             * @brief マスターボリュームを取得する。
             * @return 0.0f〜1.0f。
             */
            float GetMasterVolume() const override
            {
                return masterVolume_;
            }

            /**
             * @brief BGM ボリュームを取得する。
             * @return 0.0f〜1.0f。
             */
            float GetBgmVolume() const override
            {
                return bgmVolume_;
            }

            /**
             * @brief SE ボリュームを取得する。
             * @return 0.0f〜1.0f。
             */
            float GetSeVolume() const override
            {
                return seVolume_;
            }

            /**
             * @brief Master ミュート状態を取得する。
             * @return true: ミュート中 / false: ミュート解除。
             */
            bool IsMasterMuted() const override
            {
                return isMasterMuted_;
            }

            /**
             * @brief BGM ミュート状態を取得する。
             * @return true: ミュート中 / false: ミュート解除。
             */
            bool IsBgmMuted() const override
            {
                return isBgmMuted_;
            }

            /**
             * @brief SE ミュート状態を取得する。
             * @return true: ミュート中 / false: ミュート解除。
             */
            bool IsSeMuted() const override
            {
                return isSeMuted_;
            }

            /**
             * @brief Controller ON/OFF 状態を取得する。
             * @return true: ON / false: OFF。
             */
            bool IsControllerEnabled() const override
            {
                return isControllerEnabled_;
            }


        private:
            /**
             * @brief 音量を 0〜1 にクランプする。
             */
            static float Clamp01(float value);

        private:
            nsUI::SoundSettingScreen* pScreen_ = nullptr; //! 反映先 Screen。

            float masterVolume_ = 1.0f; //! Master 音量。
            float bgmVolume_ = 1.0f;    //! BGM 音量。
            float seVolume_ = 1.0f;     //! SE 音量。

            bool isMasterMuted_ = false;      //! Master ミュート。
            bool isBgmMuted_ = false;         //! BGM ミュート。
            bool isSeMuted_ = false;          //! SE ミュート。
            bool isControllerEnabled_ = true; //! Controller（振動など）。
        };
    } // namespace nsPresentation
} // namespace nsApp
