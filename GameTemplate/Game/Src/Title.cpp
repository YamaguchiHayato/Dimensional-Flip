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
	m_titleRender.Init("Assets/sprite/TitleScene.DDS", TITLE_WIDTH, TITLE_HEIGHT);
	
	m_titleRender.Update();
	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
	m_titleRender.Update();
}

void Title::Render(RenderContext& rc)
{
	m_titleRender.Draw(rc);
}