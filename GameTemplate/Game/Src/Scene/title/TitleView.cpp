#include "stdafx.h"

#include "Src/Scene/title/TitleInformationLayer.h"
#include "Src/Scene/title/TitleView.h"

namespace nsApp
{
    namespace nsTitle
    {
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
            pInformationLayer_ = NewGO<TitleInformationLayer>(1, "InformationLayer");

            return true;
        }

    } // namespace nsTitle
} // namespace nsApp
