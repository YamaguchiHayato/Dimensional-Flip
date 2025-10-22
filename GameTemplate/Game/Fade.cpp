#include "stdafx.h"
#include "Fade.h"

bool Fade::Start()
{
    // フェード用スプライトの初期化
    fadeSprite_.Init("Assets/fade/fadeSprite.DDS", 1600, 900);
    fadeSprite_.SetLinearWipeDrawingMode(LinearWipeDrawingMode_Round);
    fadeSprite_.SetWipeScrollSpeed(1000.0f);
    return true;
}

void Fade::Update()
{
    ChangeFadeState();

    if (fadeSprite_.GetWipeSize() < 0.0f &&
        fadeState_ == FadeEnd)
    {
        fadeInEnd_ = true;
    }

    else
    {
        fadeInEnd_ = false;
    }

    //if (fadeState_ == FadeState::Loading)
    //{
    //    LoadingProcess();
    //}


    fadeSprite_.Update();
}

void Fade::Render(RenderContext& rc)
{
    fadeSprite_.Draw(rc);
}

void Fade::LoadingProcess()
{

}

void Fade::ChangeFadeState()
{
    if (fadeTransitionFlag_ != true )
    {
        switch (fadeState_)
        {
        case FadeState::FadeStart:
             fadeSprite_.SetWipeSize(-50.0f);
             fadeTransitionFlag_ = true;
             break;

        case FadeState::FadeEnd:
             fadeSprite_.SetWipeSize(900.0f);
             fadeTransitionFlag_ = true;
             break;

        default:
            break;
        }
    }
}

void Fade::FadeTransition(FadeState fadeState)
{
    fadeState_ = fadeState;

    if (fadeState == FadeState::FadeEnd)
    {
        fadeSprite_.SetWipeScrollSpeed(-1000.0f);
    }

    else
    {
        fadeSprite_.SetWipeScrollSpeed(1000.0f);
    }

    fadeSprite_.SetFadeTransition(fadeState_);
    fadeTransitionFlag_ = false;
 

    if (fadeState_ == FadeStart)
    {
       loadingFlag_ = false;
    }
}
