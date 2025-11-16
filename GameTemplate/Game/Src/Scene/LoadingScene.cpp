#include "stdafx.h"
#include "LoadingScene.h"

bool LoadingScene::Start()
{
    // ローディング用スプライトの初期化
    loadingSprite_.Init("Assets/fade/fadeSprite.DDS", 6400, 3200);
    loadingSprite_.SetPosition(Vector3(500.0f, 0.0f, 0.0f));
    loadingSprite_.SetScale(Vector3(0.75f, 0.75f, 0.75f));
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
