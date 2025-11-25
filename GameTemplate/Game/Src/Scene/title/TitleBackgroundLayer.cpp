#include "stdafx.h"
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleLayerBase.h"

namespace
{
    // 画像の解像度。
    const float WIDTH = 2000.0f;
    const float HEIGHT = 2000.0f;

    // 回転速度。
    const float SPEED = 3.0f; // 1秒間にn(設定値)秒。
}

bool TitleBackgroundLayer::Start()
{
    // 回転用画像。
    std::string backgroundPath = Init("TitleBackground");
    titleLayerRender_.Init(backgroundPath.c_str(), WIDTH, HEIGHT);

    // 黒背景用画像。
    std::string blackPath = Init("Black");
    blackSpriteRender_.Init(blackPath.c_str(), WIDTH, HEIGHT);

    return true;
}

void TitleBackgroundLayer::Update()
{
    // 一定周期で回転。
    Rotation();

    // 画像を更新する。
    titleLayerRender_.SetRotation(rot_);
    titleLayerRender_.SetPosition(Vector3::Zero);
    titleLayerRender_.SetScale(Vector3::One); 
    titleLayerRender_.Update();

    // 黒背景画像を更新する。
    blackSpriteRender_.Update();
}

void TitleBackgroundLayer::Rotation()
{
    float angle = SPEED * g_gameTime->GetFrameDeltaTime();

    // Y軸回転。
    Quaternion deltaRot = Quaternion::Identity;
    deltaRot.SetRotation(Vector3::AxisZ, angle);

    // 新しい回転を合成させる。
    rot_ *= deltaRot;
    // 回転ベクトルを正規化する。
    rot_.Normalize();
}


void TitleBackgroundLayer::Render(RenderContext& rc)
{
    // 背景補強用
    blackSpriteRender_.Draw(rc);


    // 回転画像用
    titleLayerRender_.Draw(rc);
}
