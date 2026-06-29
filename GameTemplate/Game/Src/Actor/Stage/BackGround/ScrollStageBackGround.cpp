#include "stdafx.h"

#include <algorithm>

#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"
#include "Src/Parameter/Stage/ScrollBackGroundLayerTable.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollBackGroundLayerParameter;
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerDefinition;

    /**
     * @brief ScrollBackGroundLayerParameter から ScrollLayerDefinition に変換する。
     * @param param 変換元の ScrollBackGroundLayerParameter。
     * @param texturePath 変換後の ScrollLayerDefinition に設定するテクスチャパス。
     * @return 変換後の ScrollLayerDefinition。
     */
    ScrollLayerDefinition ToDefinition(const ScrollBackGroundLayerParameter& param, const char* texturePath)
    {
        ScrollLayerDefinition def = {};
        def.kind = param.layerKind;
        def.texturePath = texturePath;
        def.fallbackTexturePath = nullptr;
        def.parallax = param.parallax;
        def.worldY = 0.0f;
        def.worldZ = 0.0f;
        def.drawScale = 1.0f;
        def.tileWorldWidth = 512.0f;
        def.tileCount = param.tileCount;
        def.renderOrder = param.renderOrder;
        def.screenY = 0.0f;
        def.screenScale = 1.0f;
        def.screenTileWidth = 1920.0f;
        def.screenHeightRatio = param.screenHeightRatio;
        def.screenCenterY = param.screenCenterY;
        def.parallaxPixelScale = param.parallaxPixelScale;
        def.anchorBottom = param.anchorBottom;
        return def;
    }
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            StageID ScrollStageBackGround::pendingStageID_ = StageID::sInvalid;


            void ScrollStageBackGround::SetPendingStageID(StageID stageID)
            {
                // ステージIDを保留として設定する。
                pendingStageID_ = stageID;
            }


            void ScrollStageBackGround::SetStageID(StageID stageID)
            {
                /* ステージIDを直接設定する。*/
                stageId_ = stageID;
            }


            bool ScrollStageBackGround::Start()
            {
                /* 保留中のステージIDが有効であれば、それを使用する。*/
                if (stageId_ == StageID::sInvalid)
                    stageId_ = pendingStageID_;

                /* 保留中のステージIDをクリアする。*/
                pendingStageID_ = StageID::sInvalid;

                /* ステージIDがまだ無効な場合、デフォルトのチュートリアルステージに設定する。*/
                if (stageId_ == StageID::sInvalid)
                    stageId_ = StageID::sTutorialStage;

                return BuildLayers();
            }


            bool ScrollStageBackGround::BuildLayers()
            {
                /* 既存のレイヤーと定義をクリアする。*/
                layers_.clear();
                texturePathStorage_.clear();
                definitionStorage_.clear();

                /* ZPrepassDepthTexture を取得する。*/
                nsK2EngineLow::Texture* pZprepassTex = &g_renderingEngine->GetZPrepassDepthTexture();

                /* ステージIDに対応するスクロール背景レイヤー定義を取得する。*/
                const auto& params = nsSystem::ScrollBackGroundLayerTable::GetByStageID(stageId_);

                if (!params.empty())
                {
                    /* params のサイズに応じて、texturePathStorage_ と definitionStorage_ の容量を予約する。*/
                    texturePathStorage_.reserve(params.size());
                    definitionStorage_.reserve(params.size());

                    /* 各パラメータを使用して、スクロールレイヤーの定義を作成する。*/
                    for (const auto& param : params)
                    {
                        /* 各パラメータのテクスチャパスを保存し、ScrollLayerDefinition を作成する。*/
                        texturePathStorage_.push_back(param.texturePath);
                        definitionStorage_.push_back(ToDefinition(param, texturePathStorage_.back().c_str()));
                    }

                    /* definitionStorage_ のサイズに応じて、layers_ の容量を予約する。*/
                    layers_.reserve(definitionStorage_.size());

                    /* definitionStorage_ の各定義を使用して、スクロールレイヤーを構築する。*/
                    for (const auto& def : definitionStorage_)
                    {
                        /* ScrollLayer を初期化し、成功した場合は layers_ に追加する。*/
                        ScrollLayer layer;
                        if (layer.Init(def, pZprepassTex))
                            layers_.push_back(std::move(layer));
                    }
                }
                else
                {
                    /* ステージIDに対応するレイヤー定義が存在しない場合、デフォルトのレイヤー定義を使用する。*/
                    const ScrollLayerDefinition* definitions = GetNormalStageLayerDefinitions();
                    const int definitionCount = GetNormalStageLayerDefinitionCount();

                    /* デフォルトのレイヤー定義が存在しない場合、初期化に失敗する。*/
                    if (definitions == nullptr || definitionCount <= 0)
                        return false;

                    /* デフォルトのレイヤー定義を使用して、スクロールレイヤーを構築する。*/
                    layers_.reserve(static_cast<size_t>(definitionCount));

                    /* デフォルトのレイヤー定義を使用して、スクロールレイヤーを構築する。*/
                    for (int i = 0; i < definitionCount; ++i)
                    {
                        ScrollLayer layer;
                        if (layer.Init(definitions[i], pZprepassTex))
                            layers_.push_back(std::move(layer));
                    }
                }

                /* レイヤーを描画順にソートする。*/
                std::sort(layers_.begin(), layers_.end(), [](const ScrollLayer& a, const ScrollLayer& b)
                          { return a.GetRenderOrder() < b.GetRenderOrder(); });

                /* レイヤーが存在する場合は true を返す。*/
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
                (void) rc;
            }


            void ScrollStageBackGround::RenderToMainTarget(RenderContext& rc, RenderTarget& mainRT)
            {
                /* レイヤーが存在しない場合、Skip。*/
                if (layers_.empty())
                    return;

                /* メインレンダーターゲットに描画する前に、レンダーターゲットの設定を待機する。*/
                rc.WaitUntilToPossibleSetRenderTarget(mainRT);
                rc.SetRenderTargetAndViewport(mainRT);

                /* カメラのワールド座標Xと、ビューの半幅を取得する。*/
                const float cameraWorldX = GetCameraWorldX();
                const float viewHalfWidth = GetViewHalfWidth();

                /* 各レイヤーを描画する。*/
                for (auto& layer : layers_)
                    layer.Render(rc, cameraWorldX, viewHalfWidth);

                /* メインレンダーターゲットへの描画が完了するまで待機する。*/
                rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
            }


            void ScrollStageBackGround::SetOverrideTrackingPosition(const Vector3& overridePosition)
            {
                /* オーバーライドを有効化し、指定された位置を設定する。*/
                isOverrideActive_ = true;
                overrideTrackingPosition_ = overridePosition;
            }

            void ScrollStageBackGround::ClearOverride()
            {
                /* オーバーライドを無効化する。*/
                isOverrideActive_ = false;
            }


            float ScrollStageBackGround::GetCameraWorldX() const
            {
                /* オーバーライドが有効な場合、オーバーライドされた位置のX座標を返す。*/
                if (isOverrideActive_)
                    return overrideTrackingPosition_.x;

                /* オーバーライドが無効な場合、カメラのターゲット位置のX座標を返す。*/
                return g_camera3D->GetTarget().x;
            }


            float ScrollStageBackGround::GetViewHalfWidth() const
            {
                /* カメラの幅の半分を取得する。*/
                const float halfWidth = g_camera3D->GetWidth() * 0.5f;
                if (halfWidth > 1.0f)
                    return halfWidth;

                return static_cast<float>(FRAME_BUFFER_W) * 0.5f;
            }
        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
