#pragma once

#include "stdafx.h"

/**
 * @file   TransformNode.h
 * @brief  Framework 層の空間ノード。UI / Boss 付属オブジェクトで共通利用する。
 * @note   IGameObject とは独立。Engine GO の Transform とは当面共存する。
 */

namespace nsApp
{
    namespace nsFramework
    {
        /**
         * @class TransformNode
         * @brief ローカル姿勢と親子関係を保持し、ワールド行列を計算する。
         */
        class TransformNode
        {
        public:
            /* コンストラクタとデストラクタ。 */
            TransformNode();

            /**
             * @brief 親付きで生成する。
             * @param pParent 親 Transform。nullptr 可。
             */
            explicit TransformNode(TransformNode* pParent);

            /**
             * @brief ローカル座標を設定する。
             * @param position ローカル位置。
             */
            void SetLocalPosition(const Vector3& position);

            /**
             * @brief ローカル座標を取得する。
             * @return ローカル位置。
             */
            const Vector3& GetLocalPosition() const;

            /**
             * @brief ローカルスケールを設定する。
             * @param scale ローカルスケール。
             */
            void SetLocalScale(const Vector3& scale);

            /**
             * @brief ローカルスケールを取得する。
             * @return ローカルスケール。
             */
            const Vector3& GetLocalScale() const;

            /**
             * @brief ローカル回転を設定する。
             * @param rotation ローカル回転（クォータニオン）。
             */
            void SetLocalRotation(const Quaternion& rotation);

            /**
             * @brief ローカル回転を取得する。
             * @return ローカル回転。
             */
            const Quaternion& GetLocalRotation() const;

            /**
             * @brief 親 Transform を設定する。
             * @param pParent 親ノード。nullptr でルート扱い。
             */
            void SetParent(TransformNode* pParent);

            /**
             * @brief 親 Transform を取得する。
             * @return 親ノード。いなければ nullptr。
             */
            TransformNode* GetParent() const;

            /**
             * @brief ワールド行列を再計算する。
             * @note  描画・Update 前に呼ぶ。
             */
            void UpdateMatrix();

            /**
             * @brief ワールド行列を取得する。
             * @return ワールド行列。
             */
            const Matrix& GetWorldMatrix() const;

            /**
             * @brief ワールド位置を取得する。
             * @return ワールド空間上の位置。
             */
            Vector3 GetWorldPosition() const;


        private:
            /**
             * @brief ローカル行列（S * R * T）を組み立てる。
             * @return ローカル行列。
             */
            Matrix BuildLocalMatrix() const;


        private:
            Vector3 localPosition_ = Vector3::Zero;
            Vector3 localScale_ = Vector3::Zero;
            Quaternion localRotation_ = Quaternion::Identity;
            TransformNode* pParent_ = nullptr;
            Matrix worldMatrix_ = Matrix::Identity;
        };
    } // namespace nsFramework
} // namespace nsApp
