#include "stdafx.h"
#include "GameClear.h"
#include "Src/Scene/title/TitleView.h"
#include "Src/Scene/TitleScene.h"

bool GameClear::Start()
{
	m_gameClearRender.Init("Assets/sprite/GameResultScene.DDS", 1980, 1080);
	return true;
}

void GameClear::Update()
{
	//if(g_pad[0]->IsTrigger(enButtonB))
	//{
	//	//NewGO <TitleScene>(0, "titlescene");
	//	DeleteGO(this);
	//}
}

void GameClear::Render(RenderContext& rc)
{
	m_gameClearRender.Draw(rc);
}
