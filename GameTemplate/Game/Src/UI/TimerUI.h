#pragma once

#include "Src/UI/UIBase.h"

/**
 * @file   TimerUI.h
 * @brief  制限時間ゲージ（円形 UI）の表示。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class TimerUI
         * @brief 残り時間の円形メーターを描画する GO。
         */
        class TimerUI : public UIBase
        {
        public:
            TimerUI() = default;
            virtual ~TimerUI() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            const std::string InitUI(const std::string& UIname) override { return UIBase::InitUI(UIname); }

            /** @brief スケール・座標を初期値に戻す。 */
            void Reset();
        };
    } // namespace nsUI
} // namespace nsApp

using TimerUI = nsApp::nsUI::TimerUI;

namespace app
{
    namespace nsUI
    {
        using TimerUI = nsApp::nsUI::TimerUI;
    } // namespace nsUI
} // namespace app
