#pragma once

enum class EffectType : uint8_t
{
    Idle,    // エフェクト非描画中。
    Waiting, // エフェクト描画中。
    Num      // エフェクトの総数。
};

namespace app
{
    namespace effect
    {
        class DelayedEffect : public EffectEmitter
        {
        public:
            DelayedEffect() = default;
            virtual ~DelayedEffect() = default;

            bool Start();
            void Update();
            void Render(RenderContext& rc);

            // 座標とスケールを初期化する。
            void InitEffect(const Vector3& pos, const Vector3& scale);

        };

    }
}

