#pragma once

#include "Src/Presentation/Data/ISoundSettingData.h"
#include "Src/Presentation/UI/Core/UIScreen.h"

/**
 * @file   SoundSettingScreen.h
 * @brief  Sound Setting 画面（新式 UIScreen）。
 */

namespace nsApp
{
    namespace nsUI
    {
        class SoundSettingPanelComponent;
        class SoundSettingContentComponent;

        /**
         * @class SoundSettingScreen
         * @brief Setting 背景 + 音量行 / Controller を描画する UIScreen。
         */
        class SoundSettingScreen : public UIScreen
        {
        public:
            /**
             * @brief 毎フレーム更新する。
             * @param[in] deltaTime 経過秒数。
             */
            void Update(float deltaTime) override;

            /**
             * @brief 毎フレーム描画する。
             * @param[in] rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief データソースを Bind する。
             * @param[in] pData ISoundSettingData。nullptr 可。
             */
            void Bind(nsPresentation::ISoundSettingData* pData);

            /**
             * @brief UI ツリーを構築する。
             */
            void Build() override;

            /**
             * @brief データソースから各 Component へ値を委譲する。
             */
            void SyncFromDataSource();

            /**
             * @brief 選択中行を設定する。
             * @param[in] index 0:Master 1:BGM 2:SE 3:Controller。
             */
            void SetSelectedIndex(int index);


        private:
            nsPresentation::ISoundSettingData* pHudData_ = nullptr;     //!< データソース。
            SoundSettingPanelComponent* pPanelComponent_ = nullptr;     //!< 背景。
            SoundSettingContentComponent* pContentComponent_ = nullptr; //!< 中身。
            int selectedIndex_ = 0;                                     //!< 選択中行。
        };
    } // namespace nsUI
} // namespace nsApp
