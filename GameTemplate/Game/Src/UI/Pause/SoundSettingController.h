#pragma once

#include "stdint.h"

/**
 * @file   SoundSettingController.h
 * @brief  Setting 画面の開閉・行選択・音量 / トグル入力。
 */

namespace nsApp
{
    namespace nsUI
    {
        class PauseMenuUI;
        class SoundSettingScreen;
    } // namespace nsUI

    namespace nsPresentation
    {
        class SoundSettingData;
    }

    namespace nsCore
    {
        class Game;

        /**
         * @enum SoundSettingAction
         * @brief Setting 画面の戻りアクション。
         */
        enum class SoundSettingAction : uint8_t
        {
            None,       //!< 変化なし。
            BackToPause //!< ポーズメニューへ戻る。
        };

        /**
         * @class SoundSettingController
         * @brief Setting の開閉と入力を担当する（非 GO）。
         */
        class SoundSettingController
        {
        public:
            /**
             * @brief PauseMenu / Screen / Data を登録する。
             */
            void Initialize(nsUI::PauseMenuUI* pPauseMenu, nsUI::SoundSettingScreen* pScreen, nsPresentation::SoundSettingData* pData);

            void Open();
            void CloseToPause();
            bool IsOpen() const { return isOpen_; }
            SoundSettingAction Update(Game* pGame);

        private:
            void EnsureScreen();
            void ApplySelectionInput();
            void ApplyValueInput();

        private:
            nsUI::PauseMenuUI* pPauseMenu_ = nullptr;
            nsUI::SoundSettingScreen* pScreen_ = nullptr;
            nsPresentation::SoundSettingData* pData_ = nullptr;
            bool isOpen_ = false;

            int selectedIndex_ = 0; //!< 0:Master 1:BGM 2:SE 3:Controller
            static constexpr int kRowCount = 4;
            static constexpr float kVolumeStep = 0.05f;
        };
    } // namespace nsCore
} // namespace nsApp
