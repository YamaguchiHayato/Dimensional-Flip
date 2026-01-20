#pragma once
#include "Src/Actor/Actor.h"

// ボスの攻撃範囲を予測してくれるマーカークラス。


namespace app
{
    namespace production
    {
        class AttackMarker : public Actor
        {
        public:
            AttackMarker() = default;
            virtual ~AttackMarker() = default;


        // サイクル。
        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        // セッター。
        public:
            // 座標と大きさ。
            inline void SetTransform(const Vector3& position, const Vector3& scale)
            {
                // 座標のセット。
                position_ = position;
                position_.y += 5.0f; // 少し浮かせる。

                // 大きさのセット。
                scale_ = scale;
            }

            // 予兆時間。
            inline void SetDuration(float time)
            {
                maxDuration_ = time;
                timer_ = 0.0f;
            }


        private:
            // 回転演出。
            void RotationProduction();


        private:
            ModelRender markerRender_;

            Vector3 position_ = Vector3::Zero;
            Vector3 scale_ = Vector3::One;

            Quaternion rotation_ = Quaternion::Identity;


            float currentAngle_ = 0.0f;
            float timer_ = 0.0f;
            float maxDuration_ = 2.0f;
        };

    }
}
