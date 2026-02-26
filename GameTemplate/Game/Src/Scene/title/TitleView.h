#pragma once

// レイヤー分けしたタイトル画面クラス。
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleLayerBase.h"
#include "Src/Scene/title/TitleLogoLayer.h"
#include "Src/UI/Select/TitleMenu.h" 

namespace app {
    namespace title {
        class TitleInformationLayer;
    }
}

class TitleView : public IGameObject
{
public:
    TitleView() = default;
    virtual ~TitleView();


public:
    bool Start();


// セッター。
public:
    // ロゴの表示を切り替える処理。
    inline void SetShowLogo(bool isShow)
    {
        if (pLogoLayer_)
            pLogoLayer_->SetVisible(isShow);
    }

    // ロゴの表示切替
    inline void SetManualMode(bool enable)
    {
        if (pBackgroundLayer_)
            pBackgroundLayer_->SetManualMode(enable);
    }

private:
    // 各レイヤー。
    TitleBackgroundLayer* pBackgroundLayer_ = nullptr;
    TitleLogoLayer* pLogoLayer_ = nullptr;
    app::title::TitleInformationLayer* pInformationLayer_ = nullptr;

private:
    bool gameLoadFlag = false;
};
