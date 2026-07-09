#pragma once

/**
 * @file   ISoundSettingData.h
 * @brief  SoundSetting Screen が参照する設定データのインターフェース。
 */

namespace nsApp
{
    namespace nsPresentation
    {
        /**
         * @class ISoundSettingData
         * @brief Master / BGM / SE / Controller の読み取り口。
         * @note  表示/非表示は Screen::SetVisible 側が担当する。
         */
        class ISoundSettingData
        {
        public:
            virtual ~ISoundSettingData() = default;

            /**
             * @brief Master 音量を返す。
             * @return 0.0f〜1.0f。
             */
            virtual float GetMasterVolume() const = 0;

            /**
             * @brief BGM 音量を返す。
             * @return 0.0f〜1.0f。
             */
            virtual float GetBgmVolume() const = 0;

            /**
             * @brief SE 音量を返す。
             * @return 0.0f〜1.0f。
             */
            virtual float GetSeVolume() const = 0;

            /**
             * @brief Master ミュートか。
             */
            virtual bool IsMasterMuted() const = 0;

            /**
             * @brief BGM ミュートか。
             */
            virtual bool IsBgmMuted() const = 0;

            /**
             * @brief SE ミュートか。
             */
            virtual bool IsSeMuted() const = 0;

            /**
             * @brief Controller（振動など）が ON か。
             */
            virtual bool IsControllerEnabled() const = 0;
        };
    } // namespace nsPresentation
} // namespace nsApp
