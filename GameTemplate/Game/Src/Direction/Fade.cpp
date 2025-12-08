#include "stdafx.h"
#include "Fade.h"
#include "Src/Core/StageManager.h"

bool Fade::Start()
{
    fadeSprite_.Init("Assets/fade/fadeSprite.DDS", 1920, 1080);
    fadeSprite_.SetLinearWipeDrawingMode(LinearWipeDrawingMode_Round); 
    fadeSprite_.SetWipeScrollSpeed(-1000.0f);
    fadeSprite_.SetWipeSize(900.0f);


    return true;
}

void Fade::Update()
{
    // フェード状態の変更処理。
    ChangeFadeState();

    // フェードイン完了判定。
    if (fadeSprite_.GetWipeSize() < 900.0f && fadeState_ == FadeState::FadeStart)
        fadeInEnd_ = true; 

    // フェードアウト完了判定。
    if (fadeSprite_.GetWipeSize() > 0.0f && fadeState_ == FadeState::FadeEnd)
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
        case FadeState::FadeStart:
             fadeSprite_.SetWipeSize(-50.0f); // 開始位置 (隠れている)
             fadeTransitionFlag_ = true;
             break;

        case FadeState::FadeEnd:
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
    fadeState_ = FadeState::FadeEnd;

    // フェードの速度。
    fadeSprite_.SetWipeScrollSpeed(-1000.0f);

    fadeSprite_.SetFadeTransition(fadeState_);
    fadeTransitionFlag_ = false;

    fadeInEnd_ = false;
    fadeOutEnd_ = false;
}


void Fade::StartFadeIn()
{
    fadeState_ = FadeState::FadeStart;

    // フェードインの速度
    fadeSprite_.SetWipeScrollSpeed(1000.0f); 
    fadeSprite_.SetFadeTransition(fadeState_);
    fadeTransitionFlag_ = false;

    fadeInEnd_ = false;
    fadeOutEnd_ = false;
}
