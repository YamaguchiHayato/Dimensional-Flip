#include "stdafx.h"
#include "LoadingScene.h"

namespace
{
    const auto WIDHT = 500.0f;
    const auto HEIGHT = 375.0f;
}

bool LoadingScene::Start()
{
    // ローディング用スプライトの初期化
    loadingSprite_.Init("Assets/fade/loadingSprite.DDS", WIDHT, HEIGHT);
    loadingSprite_.SetPosition(Vector3(500.0f, 0.0f, 0.0f));
    loadingSprite_.SetScale(Vector3::One);
    loadingSprite_.Update();
    return true;
}

void LoadingScene::Update()
{
    loadingSprite_.Update();
}

void LoadingScene::Render(RenderContext& rc)
{
    loadingSprite_.Draw(rc);
}
