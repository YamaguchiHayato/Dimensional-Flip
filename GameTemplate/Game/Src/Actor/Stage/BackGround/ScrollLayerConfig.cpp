#include "stdafx.h"

#include "Src/Actor/Stage/BackGround/ScrollLayerConfig.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerDefinition;
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerKind;

    const ScrollLayerDefinition kNormalStageLayerDefinitions[] = {
        // Sky
        {
            ScrollLayerKind::Sky, "Assets/stage/BackGround/sky.DDS", nullptr,
            0.0f,    // parallax
            0.0f,    // worldY (2D) 画面中心
            1500.0f, // worldZ (2D) 未使用だが残す
            1.0f,    // drawScale
            1920.0f, // tileWorldWidth (2D) = FRAME_BUFFER_W
            3,       // tileCount
            0,       // renderOrder
            0.0f,    // screenY (3D) 画面中心
            1.0f,    // screenScale (3D)
            1920.0f, // screenTileWidth (3D) = FRAME_BUFFER_W
        },
        // Mountain
        {
            ScrollLayerKind::Mountain,
            "Assets/stage/BackGround/mountain.DDS",
            nullptr,
            0.45f,
            0.0f,
            1400.0f,
            1.0f,
            1920.0f,
            3,
            1,
            0.0f,
            1.0f,
            1920.0f,
        },
        // Ground
        {
            ScrollLayerKind::Ground,
            "Assets/stage/BackGround/ground.DDS",
            nullptr,
            0.75f,
            0.0f,
            1300.0f,
            1.0f,
            1920.0f,
            3,
            2,
            0.0f,
            1.0f,
            1920.0f,
        },
    };
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            const ScrollLayerDefinition* GetNormalStageLayerDefinitions()
            {
                return kNormalStageLayerDefinitions;
            }

            int GetNormalStageLayerDefinitionCount()
            {
                return static_cast<int>(_countof(kNormalStageLayerDefinitions));
            }

            const ScrollLayerDefinition& FindLayerDefinition(ScrollLayerKind kind)
            {
                for (const auto& def : kNormalStageLayerDefinitions)
                {
                    if (def.kind == kind)
                        return def;
                }

                return kNormalStageLayerDefinitions[0];
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
