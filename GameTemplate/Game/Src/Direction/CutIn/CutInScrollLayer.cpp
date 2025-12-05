#include "stdafx.h"
#include "Src/Direction/CutIn/CutInScrollLayer.h"

struct layerSize
{
    static constexpr auto WIDHT = 3000.0f;     // カットイン背景の横幅。
    static constexpr auto HEIGHT = 500.0f;     // カットイン背景の縦幅。
    static constexpr auto SHAKE_POWER = 10.0f; // 振動の強さ。
};

namespace app
{
    namespace cutIn
    {
        bool CutInScrollLayer::Start()
        {
            // スクロールレイヤーの初期化。
            const std::string layerPath = FindLayerPath("Thunder");
            layer_.Init(layerPath.c_str(), layerSize::WIDHT, layerSize::HEIGHT);

            SetScrollSpeed(Vector3(0.5f, 0.0f, 0.0f));
            uvOffset_ = Vector2::Zero;

            return true;
        }


        void CutInScrollLayer::Update()
        {
            // スクロール処理。
            Scroll();

            // 振動処理。
            Vibration();

            layer_.SetScale(Vector3::One);
            layer_.Update();
        }


        void CutInScrollLayer::Scroll()
        {
            float scroll = 1.0f / 60.0f;

            // UVを移動させる。
            // x座標。
            if (uvOffset_.x > 1.0f)
                uvOffset_.x -= 1.0f;

            // y座標。
            if (uvOffset_.y > 1.0f)
                uvOffset_.y -= 1.0f;

            layer_.SetPos(uvOffset_);
            layer_.Update();
        }


        void CutInScrollLayer::Vibration()
        {
            float offsetX = ((float) rand() / RAND_MAX - 0.5f) * 2.0f * layerSize::SHAKE_POWER;
            float offsetY = ((float) rand() / RAND_MAX - 0.5f) * 2.0f * layerSize::SHAKE_POWER;

            layer_.SetPosition(Vector3(offsetX, offsetX, 0.0f));
            layer_.Update();
        }


        void CutInScrollLayer::Render(RenderContext& rc)
        {
            layer_.Draw(rc);
        }
    }
}
