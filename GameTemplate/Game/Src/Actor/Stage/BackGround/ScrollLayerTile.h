#pragma once

#include "Src/Actor/Stage/BackGround/ScrollLayerConfig.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            /**
             * @brief スクロール背景の横タイル1枚。
             *
             * 2D: ワールド空間 + Ortho カメラ。
             * 3D: Font 数式と同じ画面空間（常に奥）。
             */
            class ScrollLayerTile
            {
            public:
                bool Init(const ScrollLayerDefinition& definition, int tileIndex, nsK2EngineLow::Texture* pZprepassDepthTexture);

                /** @brief 2D 用。ワールド座標を更新する。 */
                void UpdateWorldPosition(float cameraWorldX, const ScrollLayerDefinition& definition);

                /** @brief 3D 用。画面座標を更新する（Font と同原理）。 */
                void UpdateScreenPosition(float cameraWorldX, const ScrollLayerDefinition& definition);

                void Draw(RenderContext& rc);

                bool IsInitialized() const { return isInitialized_; }

                /**
                 * @brief 描画対象か判定する。
                 * @param is3D true なら画面 X、false ならワールド X でカリング。
                 */
                bool IsVisible(float cameraWorldX, float viewHalfWidth, float margin, bool is3D) const;

            private:
                nsK2EngineLow::Texture texture_;
                nsK2EngineLow::Texture* pZprepassDepthTexture_ = nullptr;
                nsK2EngineLow::Sprite sprite_;

                UINT textureWidth_ = 0; //!< テクスチャ幅（Init で保存）。
                Vector3 worldPosition_ = Vector3::Zero;
                float screenCenterX_ = 0.0f;   //!< 3D 画面 X（中心原点）。
                float screenTileWidth_ = 0.0f; //!< 3D タイル横幅。
                int tileIndex_ = 0;
                float tileWorldWidth_ = 0.0f;
                float drawScale_ = 1.0f;
                bool isInitialized_ = false;
            };

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
