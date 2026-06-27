#include "stdafx.h"

#include "Src/Actor/Stage/BackGround/ScrollLayerConfig.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerDefinition;
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerKind;

    const ScrollLayerDefinition kNormalStageLayerDefinitions[] = {
        // Sky — 最奥・固定
        {
            ScrollLayerKind::Sky, "Assets/stage/BackGround/sky.DDS", nullptr,
            0.0f, // parallax
            0.0f, 0.0f, 1.0f, 1920.0f,
            1,                   // tileCount: 1（全面固定で十分）
            0,                   // renderOrder
            0.0f, 1.0f, 1920.0f, // screenY, screenScale, screenTileWidth（3D用・未使用可）
            1.0f,                // screenHeightRatio
            0.0f,                // screenCenterY
            1.0f,                // parallaxPixelScale
            false,               // anchorBottom
        },
        // Mountain — 中景・ゆっくりスクロール
        {
            ScrollLayerKind::Mountain,
            "Assets/stage/BackGround/mountain.DDS",
            nullptr,
            0.35f,
            0.0f,
            0.0f,
            1.0f,
            1920.0f,
            3,
            1,
            0.0f,
            1.0f,
            1920.0f,
            0.58f, // 画面の58%の高さ
            -40.0f, // やや上
            1.0f,
            false,
        },
        // Ground — 手前・速いスクロール・下端
        {
            ScrollLayerKind::Ground,
            "Assets/stage/BackGround/ground.DDS",
            nullptr,
            0.65f,
            0.0f,
            0.0f,
            1.0f,
            1920.0f,
            3,
            2,
            0.0f,
            1.0f,
            1920.0f,
            0.40f,
            0.0f,
            1.0f,
            true, // 下端揃え
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
                /* 配列の先頭アドレスを返す。*/
                return kNormalStageLayerDefinitions;
            }

            int GetNormalStageLayerDefinitionCount()
            {
                /* 配列の要素数を返す。*/
                return static_cast<int>(_countof(kNormalStageLayerDefinitions));
            }

            const ScrollLayerDefinition& FindLayerDefinition(ScrollLayerKind kind)
            {
                /* 指定された kind に一致する定義を検索し、見つからなければ先頭の定義を返す。*/
                for (const auto& def : kNormalStageLayerDefinitions)
                {
                    /* kind が一致する場合はその定義を返す。*/
                    if (def.kind == kind)
                        return def;
                }

                /* 見つからなかった場合は先頭の定義を返す。*/
                return kNormalStageLayerDefinitions[0];
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
