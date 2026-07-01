#include "stdafx.h"

#include "Transform.h"

namespace nsApp
{
    namespace nsUI
    {
        void Transform::SetParent(Transform* parent)
        {
            /* 親を設定。*/
            parent_ = parent;
            dirty_ = true;
        }


        void Transform::SetLocalRotationDegZ(float degreesZ)
        {
            /* 回転を適応する。*/
            Quaternion rot = Quaternion::Identity;
            rot.AddRotationDegZ(degreesZ);
            SetLocalRotation(rot);
        }


        Vector3 Transform::GetWorldPosition() const
        {
            /* ワールド座標を取得する。*/
            RecalculateWorldCache();
            return cachedWorldPosition_;
        }


        Quaternion Transform::GetWorldRotation() const
        {
            /* ワールド回転を取得する。*/
            RecalculateWorldCache();
            return cachedWorldRotation_;
        }


        Vector3 Transform::GetWorldScale() const
        {
            /* ワールドスケールを取得する。*/
            RecalculateWorldCache();
            return cachedWorldScale_;
        }


        void Transform::SetWorldPosition(const Vector3& worldPosition)
        {
            /* ワールド座標を設定する。*/
            if (parent_ == nullptr)
                localPosition_ = worldPosition;

            /* 親がいる場合。*/
            else
            {
                //! 親のワールド変換の逆算は簡易版（平行移動＋スケールのみ考慮）
                const Vector3 parentPos = parent_->GetWorldPosition();
                const Vector3 parentScale = parent_->GetWorldScale();
                localPosition_.x = (worldPosition.x - parentPos.x) / parentScale.x;
                localPosition_.y = (worldPosition.y - parentPos.y) / parentScale.y;
                localPosition_.z = (worldPosition.z - parentPos.z) / parentScale.z;
            }
            dirty_ = true;
        }


        void Transform::RecalculateWorldCache() const
        {
            /* ワールド座標のキャッシュを再計算する。*/
            if (!dirty_)
                return;

            /* 親がいない場合はローカル座標をそのままワールド座標として使用する。*/
            if (parent_ == nullptr)
            {
                cachedWorldPosition_ = localPosition_;
                cachedWorldRotation_ = localRotation_;
                cachedWorldScale_ = localScale_;
            }
            else
            {
                const Vector3 parentPos = parent_->GetWorldPosition();
                const Quaternion parentRot = parent_->GetWorldRotation();
                const Vector3 parentScale = parent_->GetWorldScale();

                //! スケールは親×ローカル
                cachedWorldScale_.x = parentScale.x * localScale_.x;
                cachedWorldScale_.y = parentScale.y * localScale_.y;
                cachedWorldScale_.z = parentScale.z * localScale_.z;

                //! 回転は親×ローカル（Quaternion 積の簡易版）
                cachedWorldRotation_ = parentRot;
                cachedWorldRotation_.Multiply(localRotation_);

                //! 位置は親スケールをかけたローカル偏移を親座標に加算
                Vector3 scaledLocal;
                scaledLocal.x = localPosition_.x * parentScale.x;
                scaledLocal.y = localPosition_.y * parentScale.y;
                scaledLocal.z = localPosition_.z * parentScale.z;

                parentRot.Apply(scaledLocal);
                cachedWorldPosition_.x = parentPos.x + scaledLocal.x;
                cachedWorldPosition_.y = parentPos.y + scaledLocal.y;
                cachedWorldPosition_.z = parentPos.z + scaledLocal.z;
            }

            dirty_ = false;
        }
    } // namespace nsUI
} // namespace nsApp
