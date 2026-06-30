#pragma once

#include "Src/Actor/Character/PlayerForward.h"
#include "Src/UI/UIBase.h"

/**
 * @file   HPbarUI.h
 * @brief  画面上部の HP 表示 UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class HPbarUI
         * @brief ハート・枠・数値で HP を表示する GO。
         */
        class HPbarUI : public UIBase
        {
        public:
            HPbarUI() = default;
            virtual ~HPbarUI() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            const std::string InitUI(const std::string& UIname) override { return UIBase::InitUI(UIname); }

        private:
            void InitHPbar();
            void InitHPbar_Heart();
            void InitHPbar_Frame();
            void InitHPbar_Life();
            void SetPositions();
            void SetScales();
            void Updates();
            void UpdateHPColor();

        private:
            Player* pPlayer_ = nullptr; //!< HP 参照元。

            SpriteRender heart_; //!< ハート装飾。
            SpriteRender life_;  //!< ライフバー（予約）。
            SpriteRender flame_; //!< 枠。

            FontRender hpFont_[5]; //!< 0–3: 縁取り、4: 本体。
            wchar_t hpText_[64];   //!< 表示テキスト "9/9"。
        };
    } // namespace nsUI
} // namespace nsApp

using HPbarUI = nsApp::nsUI::HPbarUI;

namespace app
{
    namespace nsUI
    {
        using HPbarUI = nsApp::nsUI::HPbarUI;
    } // namespace nsUI
} // namespace app
