#include "stdafx.h"

#include "Src/Actor/Stage/BackGround/ScrollLayer.h"
#include "Src/Core/CameraManager.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerDefinition;

    constexpr float kCullMargin = 400.0f;
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            bool ScrollLayer::Init(const ScrollLayerDefinition& definition,
                                   nsK2EngineLow::Texture* pZprepassDepthTexture)
            {
                isInitialized_ = false;
                definition_ = definition;
                tiles_.clear();

                const int tileCount = definition.tileCount > 0 ? definition.tileCount : 1;

                for (int i = 0; i < tileCount; ++i)
                {
                    auto tile = std::make_unique<ScrollLayerTile>();
                    if (tile->Init(definition, i, pZprepassDepthTexture))
                        tiles_.push_back(std::move(tile));
                }

                isInitialized_ = !tiles_.empty();
                return isInitialized_;
            }

            void ScrollLayer::Update(float cameraWorldX)
            {
                if (!isInitialized_)
                    return;

                auto* pCamMgr = FindGO<CameraManager>("cameramanager");
                const bool is3D = (pCamMgr && pCamMgr->GetCurrentCameraMode() == CameraMode::mode3D);

                if (is3D)
                {
                    // [修正] 3D は tile[0] のみ（1枚全面）
                    if (!tiles_.empty() && tiles_[0] != nullptr && tiles_[0]->IsInitialized())
                        tiles_[0]->UpdateScreenPosition(cameraWorldX, definition_);
                    return;
                }

                for (auto& tile : tiles_)
                {
                    if (tile == nullptr || !tile->IsInitialized())
                        continue;
                    tile->UpdateWorldPosition(cameraWorldX, definition_);
                }
            }

            void ScrollLayer::Render(RenderContext& rc, float cameraWorldX, float viewHalfWidth)
            {
                if (!isInitialized_)
                    return;

                auto* pCamMgr = FindGO<CameraManager>("cameramanager");
                const bool is3D = (pCamMgr && pCamMgr->GetCurrentCameraMode() == CameraMode::mode3D);

                if (is3D)
                {
                    if (!tiles_.empty() && tiles_[0] != nullptr && tiles_[0]->IsInitialized())
                    {
                        if (tiles_[0]->IsVisible(cameraWorldX, viewHalfWidth, kCullMargin, true))
                            tiles_[0]->Draw(rc);
                    }
                    return;
                }

                for (auto& tile : tiles_)
                {
                    if (tile == nullptr || !tile->IsInitialized())
                        continue;

                    if (tile->IsVisible(cameraWorldX, viewHalfWidth, kCullMargin, false))
                        tile->Draw(rc);
                }
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
