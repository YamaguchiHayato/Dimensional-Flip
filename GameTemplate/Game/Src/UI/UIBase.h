#pragma once

/**
 * @file   UIBase.h
 * @brief  UI 派生クラスの基底 GO。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIBase
         * @brief スプライト UI の共通基底。DDS パス組み立てを提供する。
         */
        class UIBase : public IGameObject
        {
        protected:
            UIBase() = default;
            virtual ~UIBase() = default;

            virtual bool Start() = 0;
            virtual void Update() = 0;
            virtual void Render(RenderContext& rc) = 0;

            /**
             * @brief UI 用 DDS パスを組み立てる。
             * @param UIname Assets/UI/ 以下のファイル名（拡張子なし）。
             * @return フルパス文字列。
             */
            virtual const std::string InitUI(const std::string& UIname) { return "Assets/UI/" + UIname + ".DDS"; }

        protected:
            SpriteRender UISprite_; //!< 派生が使う共通スプライト（任意）。

        protected:
            /**
             * @enum enUINumber
             * @brief 数字スプライト用 index（予約）。
             */
            enum class enUINumber : uint8_t
            {
                enNumber_Zero,
                enNumber_One,
                enNumber_Two,
                enNumber_Three,
                enNumber_Four,
                enNumber_Five,
                enNumber_Six,
                enNumber_Seven,
                enNumber_Eight,
                enNumber_Nine,
                enNumber_Num,
            };
        };
    } // namespace nsUI
} // namespace nsApp

using UIBase = nsApp::nsUI::UIBase;

namespace app
{
    namespace nsUI
    {
        using UIBase = nsApp::nsUI::UIBase;
    } // namespace nsUI
} // namespace app
