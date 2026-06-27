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
             * 2D / 3D とも画面空間（中心原点）で描画する。
             * ScrollBackGround.fx により ZPrepass 未描画ピクセルへ合成する。
             */
            class ScrollLayerTile
            {
            public:
                /**
                 * @brief タイルを初期化する。
                 * @param definition レイヤー定義。
                 * @param tileIndex 横タイル番号（0 始まり）。
                 * @param pZprepassDepthTexture ZPrepass 深度テクスチャ（シェーダー t1）。
                 * @return 初期化に成功した場合 true。
                 */
                bool Init(const ScrollLayerDefinition& definition, int tileIndex, nsK2EngineLow::Texture* pZprepassDepthTexture);

                /**
                 * @brief 2D モード用の画面座標・スケールを更新する。
                 * @param cameraWorldX カメラ追従の基準ワールド X。
                 * @param definition レイヤー定義。
                 */
                void UpdateWorldPosition(float cameraWorldX, const ScrollLayerDefinition& definition);

                /**
                 * @brief 3D モード用の画面座標・スケールを更新する。
                 * @param cameraWorldX カメラ追従の基準ワールド X。
                 * @param definition レイヤー定義。
                 * @note 内部処理は 2D と同一（ApplyScreenScrollLayout）。
                 */
                void UpdateScreenPosition(float cameraWorldX, const ScrollLayerDefinition& definition);

                /**
                 * @brief タイルを描画する。
                 * @param rc レンダリングコンテキスト。
                 */
                void Draw(RenderContext& rc);

                /**
                 * @brief 初期化済みかどうかを返す。
                 * @return 初期化済みなら true。
                 */
                bool IsInitialized() const { return isInitialized_; }

                /**
                 * @brief 画面内に入っているか判定する（カリング用）。
                 * @param cameraWorldX 未使用（将来の拡張用）。
                 * @param viewHalfWidth 未使用（将来の拡張用）。
                 * @param margin カリング余白（px 相当）。
                 * @param is3D 未使用（2D/3D 共通判定）。
                 * @return 描画すべきなら true。
                 */
                bool IsVisible(float cameraWorldX, float viewHalfWidth, float margin, bool is3D) const;


            private:
                /**
                 * @brief Config に基づき画面スクロール位置・スケールを適用する。
                 * @param cameraWorldX カメラ追従の基準ワールド X。
                 * @param definition レイヤー定義。
                 */
                void ApplyScreenScrollLayout(float cameraWorldX, const ScrollLayerDefinition& definition);


            private:
                nsK2EngineLow::Texture texture_;                          //!< レイヤー DDS。
                nsK2EngineLow::Texture* pZprepassDepthTexture_ = nullptr; //!< ZPrepass 深度（t1）。
                nsK2EngineLow::Sprite sprite_;                            //!< 描画用スプライト。

                UINT textureWidth_ = 0;                                   //!< テクスチャ幅（px）。
                UINT textureHeight_ = 0;                                  //!< テクスチャ高さ（px）。
                Vector3 worldPosition_ = Vector3::Zero;                   //!< カリング用の基準 X 保持。
                float screenCenterX_ = 0.0f;                              //!< 画面中心原点での X。
                float screenTileWidth_ = 0.0f;                            //!< タイルの表示幅（px）。
                int tileIndex_ = 0;                                       //!< 横タイル番号。
                bool isInitialized_ = false;                              //!< 初期化済みフラグ。
            };

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
