#pragma once
#include "Src/Production/CutIn/CutInBase.h"

namespace app
{
    namespace cutIn
    {
        class CutInScrollLayer : public CutInBase
        {
        public:
            CutInScrollLayer() = default;
            virtual ~CutInScrollLayer() = default;

            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override;
            // 描画処理。
            void Render(RenderContext& rc) override;
            // レイヤーのパスを取得する。
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }

        private:
            // スクロール速度を設定。
            void SetScrollSpeed(const Vector3& speed)
            {
                speed_ = speed;
            }

            // スクロール処理。
            void Scroll();

            // 振動処理。
            void Vibration();


        private:
            Vector3 speed_ = Vector3::Zero;
            Vector2 uvOffset_ = Vector2::Zero;
        };

    }
}

