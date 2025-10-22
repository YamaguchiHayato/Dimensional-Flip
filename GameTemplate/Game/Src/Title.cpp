#include "stdafx.h"
#include "Src/Title.h"
#include "Game.h"
#include "Fade.h"

namespace
{
	const float TITLE_WIDTH = 1920.0f;
	const float TITLE_HEIGHT = 1080.0f;
}



bool Title::Start()
{
	titleRender_.Init("Assets/sprite/TitleScene.DDS", TITLE_WIDTH, TITLE_HEIGHT);

    fade_ = FindGO<Fade>("fade");
    fade_->FadeTransition(FadeStart);

	titleRender_.Update();
	return true;
}

void Title::Update()
{
	TitleAction();
	titleRender_.Update();
}

void Title::Render(RenderContext& rc)
{
	titleRender_.Draw(rc);
}

void Title::TitleAction()
{
    if (sceneTransitionFlag_ == false && g_pad[0]->IsTrigger(enButtonA))
    {
        fade_->FadeTransition(FadeEnd);
        sceneTransitionFlag_ = true;
    }

	if (sceneTransitionFlag_ == true && fade_->IsFadeEnd() == true)
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);

	}
}
