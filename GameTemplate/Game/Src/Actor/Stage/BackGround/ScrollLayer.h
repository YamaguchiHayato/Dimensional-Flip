#pragma once
#include "k2EnginePreCompile.h"
#include <memory>
#include <vector>

#include "Src/Actor/Stage/BackGround/ScrollLayerConfig.h"
#include "Src/Actor/Stage/BackGround/ScrollLayerTile.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            /**
             * @brief スクロール背景のレイヤー1枚分（Sky / Mountain / Ground）。
             *
             * 定義に応じた横タイル列を生成し、パララックス付きで更新・描画する。
             * 2D / 3D で更新関数のみ切り替え、タイル数・スクロール式は共通。
             */
            class ScrollLayer
            {
            public:
                /**
                 * @brief レイヤーを初期化する。
                 * @param definition レイヤー定義。
                 * @param pZprepassDepthTexture ZPrepass 深度テクスチャ。
                 * @return 初期化に成功した場合 true。
                 */
                bool Init(const ScrollLayerDefinition& definition, nsK2EngineLow::Texture* pZprepassDepthTexture);

                /**
                 * @brief カメラ位置に応じて全タイルを更新する。
                 * @param cameraWorldX カメラ基準のワールド X。
                 */
                void Update(float cameraWorldX);

                /**
                 * @brief 可視タイルを描画する。
                 * @param rc レンダリングコンテキスト。
                 * @param cameraWorldX カメラ基準のワールド X。
                 * @param viewHalfWidth 可視範囲の半幅（将来のカリング拡張用）。
                 */
                void Render(RenderContext& rc, float cameraWorldX, float viewHalfWidth);

                /**
                 * @brief 初期化済みかどうかを返す。
                 * @return 初期化済みなら true。
                 */
                bool IsInitialized() const { return isInitialized_; }

                /**
                 * @brief 描画順を返す。
                 * @return renderOrder（小さいほど奥）。
                 */
                int GetRenderOrder() const { return definition_.renderOrder; }

            private:
                ScrollLayerDefinition definition_{};                  //!< レイヤー定義のコピー。
                std::vector<std::unique_ptr<ScrollLayerTile>> tiles_; //!< 横タイル列。
                bool isInitialized_ = false;                          //!< 初期化済みフラグ。
            };

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
