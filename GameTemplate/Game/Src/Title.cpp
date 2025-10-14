#include "stdafx.h"
#include "Src/Title.h"
#include "Game.h"

namespace
{
	const float TITLE_WIDTH = 1920.0f;
	const float TITLE_HEIGHT = 1080.0f;
}



bool Title::Start()
{
	titleRender_.Init("Assets/sprite/TitleScene.DDS", TITLE_WIDTH, TITLE_HEIGHT);
	
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
	/* Aボタンでゲームスタート。*/
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		sceneTransitionFlag_ = true;
		NewGO<Game>(0, "game");
		DeleteGO(this);

	}
}