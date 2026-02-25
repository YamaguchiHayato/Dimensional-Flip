#include "stdafx.h"
#include "Src/Scene/title/TitleLogoLayer.h"

namespace
{
    // ボタン画像のステータス。
    const float BUTTON_WIDTH = 512.0f;
    const float BUTTON_HEIGHT = 512.0f;
    const Vector3 BUTTON_SCALE(Vector3::One);

    // タイトルロゴ画像のステータス。
    const float LOGO_WIDTH = 687.0f;
    const float LOGO_HEIGHT = 141.0f;

}

bool TitleLogoLayer::Start()
{
    // タイトルロゴ。
    std::string layerName = Init("title");
    titleLayerRender_.Init(layerName.c_str(), LOGO_WIDTH, LOGO_HEIGHT);


    return true;
}


void TitleLogoLayer::Update()
{
    // タイトルロゴ。
    titleLayerRender_.SetPosition({ 0.0f, 100.0f, 0.0f });
    titleLayerRender_.SetScale(Vector3(2.0f, 2.0f, 2.0f));
    titleLayerRender_.Update();

}


void TitleLogoLayer::Render(RenderContext& rc)
{
    if (isVisible_ == false)
        return;

    // タイトルロゴ描画。
    titleLayerRender_.Draw(rc);

}
