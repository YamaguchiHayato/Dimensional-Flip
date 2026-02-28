#include "stdafx.h"
#include "Fade.h"
#include "Src/Core/StageManager.h"

Fade::Fade()
{
    fadeSprite_.Init("Assets/fade/fadeSprite.DDS", 1920, 1080);
    fadeSprite_.SetLinearWipeDrawingMode(LinearWipeDrawingMode_Round);
    fadeSprite_.SetWipeScrollSpeed(-1000.0f);
    fadeSprite_.SetWipeSize(900.0f);
}


void Fade::Update()
{
    // フェード状態の変更処理。
    ChangeFadeState();

    // フェードイン完了判定。
    if (fadeSprite_.GetWipeSize() > 1300.0f && fadeState_ == FadeState::Fade_In)
        fadeInEnd_ = true; 

    // フェードアウト完了判定。
    if (fadeSprite_.GetWipeSize() < 0.0f && fadeState_ == FadeState::Fade_Out)
        fadeOutEnd_ = true;

    fadeSprite_.Update();
}

void Fade::Render(RenderContext& rc)
{
    // フェードイン完了後は描画しない。
    if (fadeInEnd_ == true) return;

    fadeSprite_.Draw(rc);

}

void Fade::ChangeFadeState()
{
    if (fadeTransitionFlag_ != true )
    {
        switch (fadeState_)
        {
        case FadeState::Fade_In:
             fadeSprite_.SetWipeSize(-50.0f); // 開始位置 (隠れている)
             fadeTransitionFlag_ = true;
             break;

        case FadeState::Fade_Out:
             fadeSprite_.SetWipeSize(900.0f); // 開始位置 (見えている)
             fadeTransitionFlag_ = true;
             break;

        default:
            break;
        }
    }
}


void Fade::StartFadeOut()
{
    fadeState_ = FadeState::Fade_Out;

    // フェードの速度。
    fadeSprite_.SetWipeScrollSpeed(1000.0f);

    fadeSprite_.SetFadeTransition(fadeState_);
    fadeTransitionFlag_ = false;

    fadeInEnd_ = false;
    fadeOutEnd_ = false;
}


void Fade::StartFadeIn()
{
    fadeState_ = FadeState::Fade_In;

    // フェードインの速度
    fadeSprite_.SetWipeScrollSpeed(1000.0f); 
    fadeSprite_.SetFadeTransition(fadeState_);
    fadeTransitionFlag_ = false;

    fadeInEnd_ = false;
    fadeOutEnd_ = false;
}
