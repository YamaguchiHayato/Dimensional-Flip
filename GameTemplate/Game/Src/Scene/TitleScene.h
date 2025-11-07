#pragma once
#include "Src/Scene/Scene.h"

class Fade;
class Title;
class TitleScene : public IScene
{
public:
	TitleScene() {};
	virtual ~TitleScene();
	bool Start() override;
	void Update() override;

private:
    void StartFadeOutToInGame();

private:
    Fade* pFade_ = nullptr;
    Title* pTitle_ = nullptr;

private:
    // 現在がフェード中かどうかを調べる。    
    bool isFadingOut = false;
};

