#pragma once

/**
 * @file   Transform.h
 * @brief  Unity の Transform に相当する、汎用の空間変換コンポーネント。
 * @details
 *  ローカル座標・回転・スケールを保持し、親子階層からワールド変換を求める。
 *  3D Actor 用の土台にも、UI 用の基底にも使える。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class Transform
         * @brief 位置・回転・スケールと親子関係を管理する。
         */
        class Transform
        {
        public:
            /* コンストラクタとデストラクタ。*/
            Transform() = default;
            virtual ~Transform() = default;


        public:
            /**
             * @brief 親 Transform を設定する。
             * @param parent 親。nullptr ならワールド直下。
             */
            void SetParent(Transform* parent);

            /**
             * @brief 親 Transform を取得する。
             * @return 親ポインタ。親が無い場合は nullptr。
             */
            inline Transform* GetParent() const { return parent_; }

            /** @brief ローカル座標を設定する。 */
            void SetLocalPosition(const Vector3& position)
            {
                localPosition_ = position;
                dirty_ = true;
            }

           /**
             * @brief ローカル座標を取得する。
             * @return ローカル座標。
            */
           inline const Vector3& GetLocalPosition() const
           {
               return localPosition_;
           }

            /**
            * @brief ローカル回転を設定する。
            * @param rotation ローカル回転。
             */
            inline void SetLocalRotation(const Quaternion& rotation)
            {
                localRotation_ = rotation;
                dirty_ = true;
            }

            /**
             * @brief ローカル回転を取得する。
             * @return ローカル回転。
             */
            inline const Quaternion& GetLocalRotation() const
            {
                return localRotation_;
            }

            /**
             * @brief Z 軸回転（度）を設定する。2D UI でよく使う。
             * @param degreesZ Z 軸まわりの角度（度）。
             */
            void SetLocalRotationDegZ(float degreesZ);

            /**
             * @brief Z 軸回転（度）を取得する。2D UI でよく使う。
             * @param scale Z 軸まわりの角度（度）。
             */
            inline void SetLocalScale(const Vector3& scale)
            {
                localScale_ = scale;
                dirty_ = true;
            }

            /**
             * @brief ローカルスケールを取得する。
             * @return ローカルスケール。
             */
            inline const Vector3& GetLocalScale() const { return localScale_; }

            /**
             * @brief 親をたどって計算したワールド座標を返す。
             * @return ワールド座標。
             */
            Vector3 GetWorldPosition() const;

            /**
             * @brief 親をたどって計算したワールド回転を返す。
             * @return ワールド回転。
             */
            Quaternion GetWorldRotation() const;

            /**
             * @brief 親をたどって計算したワールドスケールを返す。
             * @return ワールドスケール。
             */
            Vector3 GetWorldScale() const;

            /**
             * @brief ワールド座標を直接設定する（親がある場合はローカルに逆変換される）。
             * @param worldPosition 設定したいワールド座標。
             */
            void SetWorldPosition(const Vector3& worldPosition);


        protected:
            /**
             * @brief ワールド座標・回転・スケールのキャッシュを再計算する。 
             */
            void RecalculateWorldCache() const;


        protected:
            Transform* parent_ = nullptr;                     //!< 親 Transform。
            Vector3 localPosition_ = Vector3::Zero;           //!< 親から見たローカル座標。
            Quaternion localRotation_ = Quaternion::Identity; //!< 親から見たローカル回転。
            Vector3 localScale_ = Vector3::One;               //!< 親から見たローカルスケール。

            mutable bool dirty_ = true; //!< ワールドキャッシュが古いか。
            mutable Vector3 cachedWorldPosition_ = Vector3::Zero; //! < キャッシュ済みワールド座標。
            mutable Quaternion cachedWorldRotation_ = Quaternion::Identity; //! < キャッシュ済みワールド回転。
            mutable Vector3 cachedWorldScale_ = Vector3::One;               //!< キャッシュ済みワールドスケール。
        };
    } // namespace nsUI
} // namespace nsApp

using Transform = nsApp::nsUI::Transform;

namespace app
{
    namespace nsUI
    {
        using Transform = nsApp::nsUI::Transform;
    } // namespace nsUI
} // namespace app
