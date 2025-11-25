#include "stdafx.h"
#include "TitleButtonActionLayer.h"

namespace
{
    const float WIDTH = 400.0f;
    const float HEIGHT = 100.0f;
}

bool TitleButtonActionLayer::Start()
{
    // ゲームスタート画像。。
    const std::string layerName = Init("Start");
    titleLayerRender_.Init(layerName.c_str(), WIDTH, HEIGHT);

    // インストカード画像。
    const std::string howToPlayLayerName = Init("HowToPlay");
    howToPlayText_.Init(howToPlayLayerName.c_str(), WIDTH, HEIGHT);

    // ゲームエンド画像。
    const std::string endLayerName = Init("End");
    endText_.Init(endLayerName.c_str(), WIDTH, HEIGHT);

    return true;
}

void TitleButtonActionLayer::Update()
{
    // 点滅処理。
    Blinking();
    // ゲームスタート。
    titleLayerRender_.SetScale(Vector3::One);
    titleLayerRender_.SetPosition({0.0f, 100.0f, 0.0f});
    titleLayerRender_.Update();

    // インストカード。
    howToPlayText_.SetScale(Vector3::One);
    howToPlayText_.SetPosition({0.0f, -50.0f, 0.0f});
    howToPlayText_.Update();

    // ゲームエンド。
    endText_.SetScale(Vector3::One);
    endText_.SetPosition({0.0f, -250.0f, 0.0f});
    endText_.Update();
}

void TitleButtonActionLayer::Blinking()
{

}

void TitleButtonActionLayer::Render(RenderContext& rc)
{
    // ゲームスタート描画。
    titleLayerRender_.Draw(rc);
    // インストカード描画。
    howToPlayText_.Draw(rc);
    // ゲームエンド描画。
    endText_.Draw(rc);
}


