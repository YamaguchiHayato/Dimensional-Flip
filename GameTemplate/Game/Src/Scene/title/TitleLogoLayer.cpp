#include "stdafx.h"
#include "Src/Scene/title/TitleLogoLayer.h"

namespace
{
    // フォントのパラメータ。
    const Vector3 POSITION = Vector3{-700.0f, 300.0f, 0.0f};
    const auto SCALE = 3.5f;

    // 色。
    const Vector4 BASE_COLOR = Vector4::White;
    const Vector4 SHADOW_COLOR = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    
}

bool TitleLogoLayer::Start()
{
    // タイトルロゴテキストの初期化。
    InitTitleLogoText();

    return true;
}



void TitleLogoLayer::Render(RenderContext& rc)
{
    if (isVisible_ == false)
        return;

    // タイトルロゴ描画。
    titleFont_.Draw(rc);
}


void TitleLogoLayer::InitTitleLogoText()
{
    // 描画するテキスト。
    titleFont_.SetText(L"Dimension Flip");
    // 座標をセット。
    titleFont_.SetPosition(POSITION);
    // 大きさをセット。
    titleFont_.SetScale(SCALE);
    // ベースカラーをセット。
    titleFont_.SetColor(BASE_COLOR);
    // 影のパラメータをセット。
    titleFont_.SetShadowParam(true, 5.0f, SHADOW_COLOR);
}
