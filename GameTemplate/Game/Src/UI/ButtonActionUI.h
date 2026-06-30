#pragma once

#include "Src/UI/UIBase.h"

/**
 * @file   ButtonActionUI.h
 * @brief  ボス戦などで表示するボタン操作ヒント UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class ButtonActionUI
         * @brief ワールド座標に追従してボタン画像を表示する GO。
         */
        class ButtonActionUI : public UIBase
        {
        public:
            ButtonActionUI() = default;
            virtual ~ButtonActionUI() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            const std::string InitUI(const std::string& UIname) override { return UIBase::InitUI(UIname); }

            /**
             * @brief ワールド座標付近に UI を表示する。
             * @param worldPosition 基準ワールド座標。
             */
            void ShowAt(const Vector3& worldPosition);

            /** @brief 非表示にする。 */
            void Hide();

        private:
            SpriteRender buttonRender_; //!< ボタン画像。
            bool isVisible_ = false;    //!< 表示中か。
        };
    } // namespace nsUI
} // namespace nsApp

using ButtonActionUI = nsApp::nsUI::ButtonActionUI;

namespace app
{
    namespace ui
    {
        using ButtonActionUI = nsApp::nsUI::ButtonActionUI;
    } // namespace ui

    namespace nsUI
    {
        using ButtonActionUI = nsApp::nsUI::ButtonActionUI;
    } // namespace nsUI
} // namespace app
