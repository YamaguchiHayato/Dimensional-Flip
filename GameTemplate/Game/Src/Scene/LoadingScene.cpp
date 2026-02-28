#include "stdafx.h"
#include "LoadingScene.h"

namespace
{
    const auto WIDHT = 500.0f;
    const auto HEIGHT = 375.0f;
}

bool LoadingScene::Start()
{
    // 「Now Loading」の文字の初期化
    CreateLoadingFont();

    // タイマーを初期化。
    timer_ = 0.0f;
    return true;
}

void LoadingScene::Update()
{
    // 時間を取得。
    auto deltaTime = g_gameTime->GetFrameDeltaTime();

    // 規定値を超えないように補正する。
    if (deltaTime > 0.1f)
        deltaTime = 0.1f;

    timer_ += deltaTime;

    // 点滅処理。
    auto alphaValue = fabs(cosf(timer_ * 10.0f));

    // 作成した透明度をセット。
    loadingFont_.SetColor(Vector4(1.0f, 1.0f, 1.0f, alphaValue));
}

void LoadingScene::Render(RenderContext& rc)
{
    loadingFont_.Draw(rc);
}


void LoadingScene::CreateLoadingFont()
{
    // フォントの初期化
    // 描画するテキストをセット。
    loadingFont_.SetText(L"Now Loading");
    // 座標をセット。
    loadingFont_.SetPosition(Vector3(0.0f, -300.0f, 0.0f));
    // 大きさをセット。
    loadingFont_.SetScale(2.5f);
    // ベースカラーの設定。
    loadingFont_.SetColor(Vector4::White);
    // 影の濃さをセット。
    loadingFont_.SetShadowParam(true, 2.0f, Vector4::Black);
}
