#include "stdafx.h"

#include <memory>
#include "Src/Actor/Stage/BackGround/ScrollLayer.h"
#include "Src/Core/CameraManager.h"

namespace
{
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

                /* タイル列の生成。 */
                const int tileCount = definition.tileCount > 0 ? definition.tileCount : 1;

                /* tileの数だけ回す。*/
                for (int i = 0; i < tileCount; ++i)
                {
                    /* タイルを生成して初期化。失敗したらスキップ。 */
                    auto tile = std::make_unique<ScrollLayerTile>();
                    if (tile->Init(definition, i, pZprepassDepthTexture))
                        tiles_.push_back(std::move(tile));
                }

                /* タイルが1つも生成されなかった場合は初期化失敗。 */
                isInitialized_ = !tiles_.empty();
                return isInitialized_;
            }

            void ScrollLayer::Update(float cameraWorldX)
            {
                /* 初期化済みでなければ何もしない。 */
                if (!isInitialized_)
                    return;

                /* カメラモードを取得して、2D/3D の更新 API を切り替える。 */
                auto* pCamMgr = FindGO<CameraManager>("cameramanager");
                const bool is3D = (pCamMgr && pCamMgr->GetCurrentCameraMode() == CameraMode::mode3D);

                /* タイル列を更新。 */
                for (auto& tile : tiles_)
                {
                    /* タイルが初期化済みでなければスキップ。 */
                    if (tile == nullptr || !tile->IsInitialized())
                        continue;

                    /* 2D/3D に応じてタイルの位置を更新。 */
                    if (is3D)
                        tile->UpdateScreenPosition(cameraWorldX, definition_);
                    else
                        tile->UpdateWorldPosition(cameraWorldX, definition_);
                }
            }

            void ScrollLayer::Render(RenderContext& rc, float cameraWorldX, float viewHalfWidth)
            {
                /* 初期化済みでなければ何もしない。 */
                if (!isInitialized_)
                    return;

                /* カメラモードを取得して、2D/3D の描画 API を切り替える。 */
                auto* pCamMgr = FindGO<CameraManager>("cameramanager");

                /* 2D/3D の描画 API を切り替える。 */
                const bool is3D = (pCamMgr && pCamMgr->GetCurrentCameraMode() == CameraMode::mode3D);

                /* タイル列を描画。 */
                for (auto& tile : tiles_)
                {
                    /* タイルが初期化済みでなければスキップ。 */
                    if (tile == nullptr || !tile->IsInitialized())
                        continue;

                    /* カリング判定。可視範囲外のタイルは描画しない。 */
                    if (!tile->IsVisible(cameraWorldX, viewHalfWidth, kCullMargin, is3D))
                        continue;

                    /* タイルを描画。 */
                    tile->Draw(rc);
                }
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
