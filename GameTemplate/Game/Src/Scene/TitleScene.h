#pragma once
#include "Src/Scene/Scene.h"
#include "Src/UI/Select/TitleMenu.h"
#include "Src/UI/Select/ManualUI.h"

class Fade;
class TitleView;

namespace app {
    namespace nsUI {
        class TitleMenu;
    }
}

enum class TitleState
{
    Normal,
    FadingToManual,
    ManualOpen,
    FadingToMenu,
    GameStartFade,
    GameEndFade,
};

class TitleScene : public IScene
{
public:
	TitleScene() = default;
    virtual ~TitleScene();


public:
	bool Start() override;
    void Update() override;


private:
    // 入力処理を待つ。
    void WaitInputAction();


private:
    // TitleStateを更新するためのヘルパー関数。
    void UpdateTitleState();

    // NormalState時の更新処理。
    void UpdateNormalState();

    // FadingToManual時の更新処理。
    void UpdateFadingToManualState();

    // ManualOpen時の更新処理。
    void UpdateManualOpenState();

    // FadingToMenu時の更新処理。
    void UpdateFadingToMenuState();

    // GameStartFade時の更新処理。
    void UpdateGameStartFadeState();

    // GameEndFade時の更新処理。
    void UpdateGameEndFadeState();

private:
    Fade* pFade_ = nullptr;
    TitleView* pTitleView_ = nullptr;
    app::nsUI::TitleMenu* pTitleMenu_ = nullptr;
    app::nsUI::ManualUI* pManualUI_ = nullptr;

private:
    // 現在がフェード中かどうかを調べる。    
    bool isFadingOut = false;
    // 入力を取得する用変数。
    bool isUp_;      // 上入力。
    bool isDown_;    // 下入力。
    bool isDecide_;  // 決定入力。
    // ゲーム終了フラグ。
    bool isGameEndRequested_ = false;

    int nextSceneID_ = -1;

    TitleMenuType selectType_;
    TitleState titleState_ = TitleState::Normal;
};

