#pragma once
#include "Src/Scene/Scene.h"
#include "Src/UI/Select/TitleMenu.h"

class Fade;
class TitleView;

namespace app {
    namespace nsUI {
        class TitleMenu;
    }
}

class TitleScene : public IScene
{
public:
	TitleScene() = default;
    virtual ~TitleScene();


public:
	bool Start() override;
	void Update() override;


private:
    void StartFadeOutToInGame();

    // 入力処理を待つ。
    void WaitInputAction();

    // ゲーム開始処理。
    void StartToInGame();

    // ゲーム終了処理。
    void RequestGameEnd();

private:
    Fade* pFade_ = nullptr;
    TitleView* pTitleView_ = nullptr;
    app::nsUI::TitleMenu* pTitleMenu_ = nullptr;

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
};

