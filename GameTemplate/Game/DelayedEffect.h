#pragma once

//enum class EffectType : uint8_t
//{
//    Idle,    // エフェクト非描画中。
//    Waiting, // エフェクト描画中。
//    Num      // エフェクトの総数。
//};

namespace app
{
    namespace effect
    {
        // TODO : 将来的にエフェクトに差し替える。
        class DelayedEffect : public IGameObject
        {
        public:
            DelayedEffect() = default;
            virtual ~DelayedEffect() = default;

            bool Start();
            void Update();
            void Render(RenderContext& rc);

            // 座標とスケールを初期化する。
            void InitEffect(const Vector3& pos, const Vector3& scale);

        private:
            SpriteRender warrningSprite_;

            Vector3 initPos_ = Vector3::Zero;
            Vector3 initScale_ = Vector3::One;
        };

    }
}

