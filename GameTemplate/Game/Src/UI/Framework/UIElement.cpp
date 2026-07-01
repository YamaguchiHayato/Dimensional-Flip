#include "stdafx.h"

#include "UIElement.h"

namespace nsApp
{
    namespace nsUI
    {
        UIElement::UIElement(const char* name) : name_(name) {}


        void UIElement::AddChild(std::shared_ptr<UIElement> child)
        {
            /* 親にTransformの情報が慣れければ終了。*/
            if (!child)
                return;

            /* 子のTransformの親を設定する。*/
            child->GetTransform().SetParent(&transform_);
            children_.push_back(child);
        }


        void UIElement::SetActive(bool active)
        {
            /* アクティブ状態を設定する。*/
            isActive_ = active;

            /* 画像があれば表示状態を設定する。*/
            if (image_)
                image_->SetVisible(active);

            /* 子要素にも同じアクティブ状態を設定する。*/
            for (auto& child : children_)
                child->SetActive(active);
        }


        void UIElement::UpdateRecursive()
        {
            /* アクティブでなければ更新しない。*/
            if (!isActive_)
                return;

            /* 画像があればTransformを更新する。*/
            if (image_)
            {
                /* 画像のTransformの親を設定する。*/
                image_->GetTransform().SetParent(&transform_);
                image_->Update();
            }

            /* 子要素を再帰的に更新する。*/
            for (auto& child : children_)
                child->UpdateRecursive();
        }


        void UIElement::DrawRecursive(RenderContext& rc) const
        {
            /* アクティブでなければ描画しない。*/
            if (!isActive_)
                return;

            /* 画像があれば描画する。*/
            if (image_)
                image_->Draw(rc);

            /* 子要素を再帰的に描画する。*/
            for (const auto& child : children_)
                child->DrawRecursive(rc);
        }
    } // namespace nsUI
} // namespace nsApp
