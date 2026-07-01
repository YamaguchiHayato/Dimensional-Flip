#include "stdafx.h"

#include "UITransform.h"

namespace nsApp
{
    namespace nsUI
    {
        float UITransform::referenceWidth_ = 1920.0f;
        float UITransform::referenceHeight_ = 1080.0f;


        void UITransform::SetReferenceResolution(float width, float height)
        {
            /* 解像度を参照する。*/
            referenceWidth_ = width;
            referenceHeight_ = height;
        }


        void UITransform::SetAnchorPreset(UIAnchorPreset preset)
        {
            /* アンカー位置を設定。*/
            anchorPreset_ = preset;
            RebuildLayout();
        }


        void UITransform::SetAnchoredPosition(const Vector2& offset)
        {
            /* アンカー点からのオフセットを設定。*/
            anchoredPosition_ = offset;
            RebuildLayout();
        }


        void UITransform::SetSize(const Vector2& size)
        {
            /* サイズを設定。*/
            size_ = size;
        }


        void UITransform::SetPivot(const Vector2& pivot)
        {
            /* ピボットを設定。*/
            pivot_ = pivot;
        }


        void UITransform::RebuildLayout()
        {
            /* アンカー点を計算。*/
            const Vector2 anchorPoint = CalcAnchorPoint();

            /* アンカー点 + オフセットでローカル座標を決定。*/
            Vector3 localPos;
            localPos.x = anchorPoint.x + anchoredPosition_.x;
            localPos.y = anchorPoint.y + anchoredPosition_.y;
            localPos.z = 0.0f;
            SetLocalPosition(localPos);
        }


        Vector2 UITransform::CalcAnchorPoint() const
        {
            /* アンカー位置に応じて、参照解像度の座標を返す。*/
            const float halfW = referenceWidth_ * 0.5f;
            const float halfH = referenceHeight_ * 0.5f;

            /* アンカー位置に応じて、参照解像度の座標を返す。*/
            switch (anchorPreset_)
            {
            case UIAnchorPreset::TopLeft:
                return {-halfW, halfH};
            case UIAnchorPreset::TopCenter:
                return {0.0f, halfH};
            case UIAnchorPreset::TopRight:
                return {halfW, halfH};
            case UIAnchorPreset::MiddleLeft:
                return {-halfW, 0.0f};
            case UIAnchorPreset::MiddleRight:
                return {halfW, 0.0f};
            case UIAnchorPreset::BottomLeft:
                return {-halfW, -halfH};
            case UIAnchorPreset::BottomCenter:
                return {0.0f, -halfH};
            case UIAnchorPreset::BottomRight:
                return {halfW, -halfH};
            case UIAnchorPreset::Center:
            default:
                return {0.0f, 0.0f};
            }
        }


        void UITransform::ApplyTo(SpriteRender& sprite) const
        {
            /* SpriteRender にワールド変換を適用する。*/
            sprite.SetPosition(GetWorldPosition());
            sprite.SetRotation(GetWorldRotation());
            sprite.SetScale(GetWorldScale());
            sprite.SetPivot(pivot_);
        }


        void UITransform::ApplyTo(FontRender& font) const
        {
            /* FontRender にワールド変換を適用する。*/
            const Vector3 worldPos = GetWorldPosition();
            font.SetPosition(worldPos);
            font.SetPivot(pivot_.x, pivot_.y);

            //! FontRender は Z 回転のみ。ワールド回転の Z 成分を使う簡易版
            const float scale = GetWorldScale().x;
            font.SetScale(scale);
        }
    } // namespace nsUI
} // namespace nsApp
