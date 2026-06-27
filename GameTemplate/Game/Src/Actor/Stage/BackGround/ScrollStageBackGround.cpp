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
                layers_.clear();
                const ScrollLayerDefinition* definitions = GetNormalStageLayerDefinitions();
                const int definitionCount = GetNormalStageLayerDefinitionCount();
                if (definitions == nullptr || definitionCount <= 0)
                    return false;
                // [追加] Font/CompositeBackground と同じ ZPrepass 深度
                nsK2EngineLow::Texture* pZprepassTex = &g_renderingEngine->GetZPrepassDepthTexture();
                layers_.reserve(static_cast<size_t>(definitionCount));
                for (int i = 0; i < definitionCount; ++i)
                {
                    ScrollLayer layer;
                    // [修正] Init に ZPrepass を渡す
                    if (layer.Init(definitions[i], pZprepassTex))
                        layers_.push_back(std::move(layer));
                }
                std::sort(layers_.begin(), layers_.end(), [](const ScrollLayer& a, const ScrollLayer& b)
                          { return a.GetRenderOrder() < b.GetRenderOrder(); });
                return !layers_.empty();
            }


            void ScrollStageBackGround::Update()
            {
                const float cameraWorldX = GetCameraWorldX();

                for (auto& layer : layers_)
                    layer.Update(cameraWorldX);
            }

            void ScrollStageBackGround::Render(RenderContext& rc)
            {
                (void)rc;
            }


            void ScrollStageBackGround::RenderToMainTarget(RenderContext& rc, RenderTarget& mainRT)
            {
                if (layers_.empty())
                    return;

                rc.WaitUntilToPossibleSetRenderTarget(mainRT);
                rc.SetRenderTargetAndViewport(mainRT); // Font と同じ

                const float cameraWorldX = GetCameraWorldX();
                const float viewHalfWidth = GetViewHalfWidth();

                for (auto& layer : layers_)
                    layer.Render(rc, cameraWorldX, viewHalfWidth);

                rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
            }


            void ScrollStageBackGround::SetOverrideTrackingPosition(const Vector3& overridePosition)
            {
                isOverrideActive_ = true;
                overrideTrackingPosition_ = overridePosition;
            }

            void ScrollStageBackGround::ClearOverride()
            {
                isOverrideActive_ = false;
            }

            float ScrollStageBackGround::GetCameraWorldX() const
            {
                if (isOverrideActive_)
                    return overrideTrackingPosition_.x;

                return g_camera3D->GetTarget().x;
            }

            float ScrollStageBackGround::GetViewHalfWidth() const
            {
                const float halfWidth = g_camera3D->GetWidth() * 0.5f;
                if (halfWidth > 1.0f)
                    return halfWidth;

                return static_cast<float>(FRAME_BUFFER_W) * 0.5f;
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nspp
