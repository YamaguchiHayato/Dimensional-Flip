#include "stdafx.h"
#include "Loading.h"

bool Loading::Start()
{
    // ローディング用スプライトの初期化
    loadingSprite_.Init("Assets/fade/loadingSprite.DDS", 3200, 1600);
    loadingSprite_.SetPosition(Vector3(475.0f, -345.0f, 0.0f));
    loadingSprite_.SetScale(Vector3(0.75f, 0.75f, 0.75f));
    loadingSprite_.Update();
    return true;
}
