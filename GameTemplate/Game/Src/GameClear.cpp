#include "stdafx.h"
#include "GameClear.h"
#include "Src/Title.h"

bool GameClear::Start()
{
	m_gameClearRender.Init("Assets/sprite/GameResultScene.DDS", 1980, 1080);
	return true;
}

void GameClear::Update()
{
	if(g_pad[0]->IsTrigger(enButtonB))
	{
		NewGO <Title>(0, "title");
		DeleteGO(this);
	}
}

void GameClear::Render(RenderContext& rc)
{
	m_gameClearRender.Draw(rc);
}