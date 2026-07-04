#include "stdafx.h"

#include "Src/Presentation/UI/Components/UISpriteView.h"

namespace nsApp
{
    namespace nsUI
    {
        void UISpriteView::Initialize(const char* texturePath, float width, float height)
        {
            sprite_.Init(texturePath, width, height);
            isInitialized_ = true;
        }


        void UISpriteView::SetScale(const Vector3& scale)
        {
            drawScale_ = scale;
        }


        void UISpriteView::SetLocalOffset(const Vector3& offset)
        {
            localOffset_ = offset;
        }


        void UISpriteView::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            if (!isInitialized_)
                return;

            /* 所有者のワールド位置 + ローカルオフセットを描画位置に使う */
            Vector3 worldPos;
            worldPos.x = ownerWorld.m[3][0] + localOffset_.x;
            worldPos.y = ownerWorld.m[3][1] + localOffset_.y;
            worldPos.z = ownerWorld.m[3][2] + localOffset_.z;

            sprite_.SetPosition(worldPos);
            sprite_.SetScale(drawScale_);
            sprite_.Update();
            sprite_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
