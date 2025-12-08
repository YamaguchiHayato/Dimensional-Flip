#include "stdafx.h"

// ゲームクラス。
#include "Src/Core/Game.h"

// 演出クラス。
#include "Src/Direction/Fade.h"

// タイトルレイヤークラス。
#include "Src/Scene/title/TitleView.h"
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleButtonActionLayer.h"
#include "Src/Scene/title/TitleLogoLayer.h"
#include "Src/Scene/title/TitleLayerBase.h"

TitleView::~TitleView()
{
    DeleteGO(pBackgroundLayer_);
    DeleteGO(pLogoLayer_);
    DeleteGO(pButtonActionLayer);
}

bool TitleView::Start()
{
    // 各レイヤーの生成。
    pBackgroundLayer_ = NewGO<TitleBackgroundLayer>(0, "titleBackgroundLayer");
//    pButtonActionLayer = NewGO<TitleButtonActionLayer>(1, "titlebuttonnactionlayer");
    pLogoLayer_ = NewGO<TitleLogoLayer>(1, "titlelogolayer");

    return true;    
}
