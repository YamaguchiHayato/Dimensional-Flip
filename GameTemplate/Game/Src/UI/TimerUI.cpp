#include "stdafx.h"
#include "Src/UI/TimerUI.h"

namespace
{
	const float UI_WIDTH = 300.0f;
	const float UI_HEIGHT = 300.0f;

	const Vector3 UI_POS{00.0f, 450.0f,0.0f};
	const Vector3 UI_SCALE{ 1.25f, 1.25f, 1.25f };
}


bool TimerUI::Start()
{
	const std::string TimerUIPath = InitUI("progressBar/UI_Time");
	UISprite_.Init(TimerUIPath.c_str(), UI_WIDTH, UI_HEIGHT);

	UISprite_.SetScale(UI_SCALE);
	UISprite_.SetPosition(UI_POS);
	UISprite_.Update();
	return true;
}

void TimerUI::Update()
{
	UISprite_.SetPosition(UI_POS);
	UISprite_.SetScale(UI_SCALE);
	UISprite_.Update();
}

void TimerUI::Render(RenderContext& rc)
{
	UISprite_.Draw(rc);
}

