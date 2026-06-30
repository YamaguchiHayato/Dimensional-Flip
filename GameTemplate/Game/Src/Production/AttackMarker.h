#pragma once

#include "Src/Actor/Actor.h"

/**
 * @file   AttackMarker.h
 * @brief  ボス攻撃の予兆マーカー。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class AttackMarker
         * @brief 攻撃範囲を地面に表示する予兆マーカー。
         */
        class AttackMarker : public Actor
        {
        public:
            /* コンストラクタとデストラクタ。*/
            AttackMarker() = default;
            virtual ~AttackMarker() = default;


        public:
            /**
             * @brief 初期化処理。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。
             */
            void Update() override;

            /**
             * @brief 描画処理。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief トランスフォームをセットする。
             * @param position 座標。
             * @param scale スケール。
             */
            inline void SetTransform(const Vector3& position, const Vector3& scale)
            {
                position_ = position;
                position_.y += 5.0f;
                scale_ = scale;
            }

            /**
             * @brief マーカーの回転アニメーションの継続時間をセットする。
             * @param time 継続時間（秒）。
             */
            inline void SetDuration(float time)
            {
                maxDuration_ = time;
                timer_ = 0.0f;
            }


        private:
            /**
             * @brief マーカーの回転アニメーションを更新する。
             */
            void RotationProduction();


        private:
            ModelRender markerRender_; //! < マーカーのモデルレンダラー

            Vector3 position_ = Vector3::Zero; //! < マーカーの座標。
            Vector3 scale_ = Vector3::One;     //! < マーカーのスケール。
            Quaternion rotation_ = Quaternion::Identity; //! < マーカーの回転。

            float currentAngle_ = 0.0f; //! < マーカーの現在の回転角度。
            float timer_ = 0.0f;        //! < マーカーの回転アニメーションの経過時間。
            float maxDuration_ = 2.0f;  //! < マーカーの回転アニメーションの最大継続時間。
        };
    } // namespace nsProduction
} // namespace nsApp

using AttackMarker = nsApp::nsProduction::AttackMarker;
