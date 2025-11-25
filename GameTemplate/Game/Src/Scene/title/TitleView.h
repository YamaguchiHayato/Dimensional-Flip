#pragma once

// レイヤー分けしたタイトル画面クラス。
#include "Src/Scene/title/TitleLayerBase.h"
#include "Src/Scene/title/TitleButtonActionLayer.h"
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleLogoLayer.h"


class Fade;
class TitleView : public IGameObject
{
public:
    TitleView() {};
    virtual ~TitleView();

    bool Start();
    void Update() {};
    void Render(RenderContext& rc) {};
    inline const std::string Init(const std::string layerName)
    {
        const std::string path = "Assets/title/" + layerName + ".DDS";
        return path;
    }

// セッター。
public:
    inline const void SetGameLooadFlag(bool flag) { gameLoadFlag = flag; }

// ゲッター。
public:
    inline bool GetGameLoadFlag() { return gameLoadFlag; }

private:
    // フェード。
    Fade* fade_ = nullptr;
    // 各レイヤー。
    TitleLayerBase* pBackgroundLayer_ = nullptr;  // 背景画像。
    TitleLayerBase* pButtonActionLayer = nullptr; // ボタンアクション。
    TitleLayerBase* pLogoLayer_ = nullptr;        // ロゴ。

private:
    bool gameLoadFlag = false;
};
