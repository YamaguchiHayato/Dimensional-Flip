#include "stdafx.h"

#include "TransformNode.h"

namespace nsApp
{
    namespace nsFramework
    {
        TransformNode::TransformNode()
            : localPosition_(Vector3::Zero), localScale_(Vector3::One), localRotation_(Quaternion::Identity),
              pParent_(nullptr), worldMatrix_(Matrix::Identity)
        {
        }


        TransformNode::TransformNode(TransformNode* pParent)
            : localPosition_(Vector3::Zero), localScale_(Vector3::One), localRotation_(Quaternion::Identity),
              pParent_(pParent), worldMatrix_(Matrix::Identity)
        {
        }


        void TransformNode::SetLocalPosition(const Vector3& position)
        {
            localPosition_ = position;
        }


        const Vector3& TransformNode::GetLocalPosition() const
        {
            return localPosition_;
        }


        void TransformNode::SetLocalScale(const Vector3& scale)
        {
            localScale_ = scale;
        }


        const Vector3& TransformNode::GetLocalScale() const
        {
            return localScale_;
        }


        void TransformNode::SetLocalRotation(const Quaternion& rotation)
        {
            localRotation_ = rotation;
            localRotation_.Normalize();
        }


        const Quaternion& TransformNode::GetLocalRotation() const
        {
            return localRotation_;
        }


        void TransformNode::SetParent(TransformNode* pParent)
        {
            pParent_ = pParent;
        }


        TransformNode* TransformNode::GetParent() const
        {
            return pParent_;
        }


        void TransformNode::UpdateMatrix()
        {
            /* ローカル行列を組み立てる */
            const Matrix local = BuildLocalMatrix();

            /* 親がいれば親のワールド行列と合成する */
            if (pParent_ != nullptr)
            {
                pParent_->UpdateMatrix();
                worldMatrix_ = local * pParent_->GetWorldMatrix();
            }
            else
                worldMatrix_ = local;
        }


        const Matrix& TransformNode::GetWorldMatrix() const
        {
            return worldMatrix_;
        }


        Vector3 TransformNode::GetWorldPosition() const
        {
            /* ワールド行列の平行移動成分を取り出す */
            Vector3 pos;
            pos.x = worldMatrix_.m[3][0];
            pos.y = worldMatrix_.m[3][1];
            pos.z = worldMatrix_.m[3][2];
            return pos;
        }


        Matrix TransformNode::BuildLocalMatrix() const
        {
            /*
             * 暫定: スケール + 平行移動のみ
             * 回転を入れる場合は BossUIBase::UpdateMatrix と同じ API に合わせる
             */
            Matrix m = Matrix::Identity;

            m.m[0][0] = localScale_.x;
            m.m[1][1] = localScale_.y;
            m.m[2][2] = localScale_.z;

            m.m[3][0] = localPosition_.x;
            m.m[3][1] = localPosition_.y;
            m.m[3][2] = localPosition_.z;

            return m;
        }
    } // namespace nsFramework
} // namespace nsApp
