#include "stdafx.h"

#include "Src/Scene/title/TitleView.h"
#include "Src/Scene/title/TitleInformationLayer.h"


TitleView::~TitleView()
{
    DeleteGO(pBackgroundLayer_);
    DeleteGO(pLogoLayer_);
    DeleteGO(pInformationLayer_);
}

bool TitleView::Start()
{
    // タイトル背景。
    pBackgroundLayer_ = NewGO<TitleBackgroundLayer>(0, "titleBackgroundLayer");

    // タイトルロゴ。
    pLogoLayer_ = NewGO<TitleLogoLayer>(1, "titleLogoLayer");

    // 権利クレジットとバージョン数の表記。
    pInformationLayer_ = NewGO<app::title::TitleInformationLayer>(1, "InformationLayer");

    return true;
}


