#pragma once

#include "IUIComponent.h"

/**
 * @file   UITextView.h
 * @brief  FontRender をラップする UI Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UITextView
         * @brief テキストを描画する IUIComponent。
         */
        class UITextView : public IUIComponent
        {
        public:
            /**
             * @brief ローカルオフセットを設定する。
             * @param offset 親 Entity からのオフセット。
             */
            void SetLocalOffset(const Vector3& offset);

            /**
             * @brief スケールを設定する。
             * @param scale フォントスケール。
             */
            void SetScale(float scale);

            /**
             * @brief 色を設定する。
             * @param color 描画色。
             */
            void SetColor(const Vector4& color);

            /**
             * @brief 表示文字列を設定する。
             * @param text 表示文字列。
             */
            void SetText(const wchar_t* text);

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             * @param ownerWorld 親 Entity のワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;

        private:
            FontRender font_; //! < フォントレンダラー。
            Vector3 localOffset_ = Vector3::Zero; //! < 親 Entity からのローカルオフセット。
            Vector3 drawPosition_ = Vector3::Zero;//! < 描画位置。親 Entity のワールド行列と localOffset_ から計算される。
        };
    } // namespace nsUI
} // namespace nsApp
