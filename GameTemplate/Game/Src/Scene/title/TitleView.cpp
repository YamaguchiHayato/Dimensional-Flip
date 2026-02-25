#include "stdafx.h"

#include "Src/Core/Game.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/title/TitleView.h"



TitleView::~TitleView()
{
    DeleteGO(pBackgroundLayer_);
    DeleteGO(pLogoLayer_);
}

bool TitleView::Start()
{
    // タイトル背景。
    pBackgroundLayer_ = NewGO<TitleBackgroundLayer>(0, "titleBackgroundLayer");

    // タイトルロゴ。
    pLogoLayer_ = NewGO<TitleLogoLayer>(1, "titleLogoLayer");

    return true;
}


