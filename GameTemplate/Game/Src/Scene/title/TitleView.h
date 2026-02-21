#pragma once

// レイヤー分けしたタイトル画面クラス。
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleLayerBase.h"
#include "Src/Scene/title/TitleLogoLayer.h"
#include "Src/UI/Select/TitleMenu.h" // パスは環境に合わせてください

class Fade;
class TitleView : public IGameObject
{
public:
    TitleView() {};
    virtual ~TitleView();

    bool Start();
    void Update();
    void Render(RenderContext& rc) {};

private:
    // 各レイヤー。
    TitleLayerBase* pBackgroundLayer_ = nullptr;
    TitleLayerBase* pLogoLayer_ = nullptr;
    app::nsUI::TitleMenu* pTitleMenu_ = nullptr;
    Fade* m_pFade = nullptr; // フェードオブジェクト


private:
    TitleMenuType m_selectedMenu = TitleMenuType::Max; // 選択された項目


private:
    bool m_isStartedFadeOut = false; // フェードアウトを開始したか

    bool gameLoadFlag = false;
};
