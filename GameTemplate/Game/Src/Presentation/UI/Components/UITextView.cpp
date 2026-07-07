#include "stdafx.h"

#include "UITextView.h"

namespace nsApp
{
    namespace nsUI
    {
        void UITextView::SetLocalOffset(const Vector3& offset)
        {
            /* 親 Entity からのローカルオフセットを設定する。 */
            localOffset_ = offset;
        }


        void UITextView::SetScale(float scale)
        {
            /* フォントのスケールを設定する。 */
            font_.SetScale(scale);
        }


        void UITextView::SetColor(const Vector4& color)
        {
            /* フォントの描画色を設定する。 */
            font_.SetColor(color);
        }


        void UITextView::SetText(const wchar_t* text)
        {
            /* nullptr チェックを行い、nullptr でなければフォントにテキストを設定する。 */
            if (text != nullptr)
                font_.SetText(text);
        }


        void UITextView::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            /* 親 Entity のワールド行列にローカルオフセットを加算して描画位置を計算する。 */
            drawPosition_.x = ownerWorld.m[3][0] + localOffset_.x;
            drawPosition_.y = ownerWorld.m[3][1] + localOffset_.y;
            drawPosition_.z = ownerWorld.m[3][2] + localOffset_.z;

            /* フォントの描画位置を設定して描画する。 */
            font_.SetPosition(drawPosition_);
            font_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
