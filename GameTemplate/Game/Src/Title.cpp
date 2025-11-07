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

    return true;
}

void Title::Update()
{
    titleRender_.SetPosition({ 0.0f, 0.0f, 0.0f });

    titleRender_.SetScale({ 1.0f, 1.0f, 1.0f });
	titleRender_.Update();
}

void Title::Render(RenderContext& rc)
{
	titleRender_.Draw(rc);
}

