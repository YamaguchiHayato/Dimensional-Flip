#include "stdafx.h"

#include <algorithm>

#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            bool ScrollStageBackGround::Start()
            {
                /* 初期化。*/
                layers_.clear();

                /* スクロール背景のレイヤー定義を取得。*/
                const ScrollLayerDefinition* definitions = GetNormalStageLayerDefinitions();
                const int definitionCount = GetNormalStageLayerDefinitionCount();

                /* 定義が存在しない場合は初期化失敗。*/
                if (definitions == nullptr || definitionCount <= 0)
                    return false;

                /* ZPrepass 深度テクスチャを取得。*/
                nsK2EngineLow::Texture* pZprepassTex = &g_renderingEngine->GetZPrepassDepthTexture();
                layers_.reserve(static_cast<size_t>(definitionCount));

                /* 各レイヤーを初期化し、描画順でソート。*/
                for (int i = 0; i < definitionCount; ++i)
                {
                    /* レイヤーを初期化。*/
                    ScrollLayer layer;

                    /* 初期化に成功した場合のみ layers_ に追加。*/
                    if (layer.Init(definitions[i], pZprepassTex))
                        layers_.push_back(std::move(layer));
                }

                /* 描画順でソート（renderOrder が小さいほど奥）。*/
                std::sort(layers_.begin(), layers_.end(), [](const ScrollLayer& a, const ScrollLayer& b)
                 {
                        return a.GetRenderOrder() < b.GetRenderOrder();
                 });

                /* 初期化に成功した場合は true を返す。*/
                return !layers_.empty();
            }


            void ScrollStageBackGround::Update()
            {
                /* カメラのワールド X 座標を取得。*/
                const float cameraWorldX = GetCameraWorldX();

                /* 各レイヤーを更新。*/
                for (auto& layer : layers_)
                    layer.Update(cameraWorldX);
            }

            void ScrollStageBackGround::Render(RenderContext& rc)
            {
                /* 2D カメラモードでのみ描画。*/
                (void)rc;
            }


            void ScrollStageBackGround::RenderToMainTarget(RenderContext& rc, RenderTarget& mainRT)
            {
                /* レイヤーが存在しない場合は描画せずに戻る。*/
                if (layers_.empty())
                    return;

                /* メインレンダーターゲットに描画する前に、描画可能になるまで待機。*/
                rc.WaitUntilToPossibleSetRenderTarget(mainRT);
                rc.SetRenderTargetAndViewport(mainRT); 

                /* カメラのワールド X 座標と可視範囲の半幅を取得。*/
                const float cameraWorldX = GetCameraWorldX();
                const float viewHalfWidth = GetViewHalfWidth();

                /* 各レイヤーを描画。*/
                for (auto& layer : layers_)
                    layer.Render(rc, cameraWorldX, viewHalfWidth);

                /* 描画完了まで待機。*/
                rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
            }


            void ScrollStageBackGround::SetOverrideTrackingPosition(const Vector3& overridePosition)
            {
                /* 追従上書きを有効化し、上書き座標を保存。*/
                isOverrideActive_ = true;
                overrideTrackingPosition_ = overridePosition;
            }


            void ScrollStageBackGround::ClearOverride()
            {
                /* 追従上書きを無効化。*/
                isOverrideActive_ = false;
            }


            float ScrollStageBackGround::GetCameraWorldX() const
            {
                /* 追従上書きが有効な場合は上書き座標を返す。*/
                if (isOverrideActive_)
                    return overrideTrackingPosition_.x;

                /* 追従上書きが無効な場合はカメラのターゲット X 座標を返す。*/
                return g_camera3D->GetTarget().x;
            }


            float ScrollStageBackGround::GetViewHalfWidth() const
            {
                /* カメラの幅の半分を取得。*/
                const float halfWidth = g_camera3D->GetWidth() * 0.5f;
                if (halfWidth > 1.0f)
                    return halfWidth;

                /* カメラの幅が 1.0 未満の場合はフレームバッファの半幅を返す。*/
                return static_cast<float>(FRAME_BUFFER_W) * 0.5f;
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nspp
