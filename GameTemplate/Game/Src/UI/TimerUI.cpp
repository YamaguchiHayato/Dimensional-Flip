#include "stdafx.h"
#include "Src/UI/TimerUI.h"
#include "Src/Core/StageManager.h"

namespace
{
	const auto WIDTH = 1210.0f;
	const auto HEIGHT = 720.0f;

	const Vector3 UI_POS{-870.0f, 360.0f,0.0f};
	const Vector3 UI_SCALE(Vector3(0.125f, 0.125f, 0.125f));
}


bool TimerUI::Start()
{
	const std::string TimerUIPath = InitUI("progressBar/Time");
	UISprite_.Init(TimerUIPath.c_str(), WIDTH, HEIGHT);

	UISprite_.SetPosition(UI_POS);
	UISprite_.Update();
	return true;
}

void TimerUI::Update()
{
    auto* pStageManager = app::core::StageManager::GetInstance();
    if (pStageManager)
    {
        if (pStageManager->GetCurrentStageID() == StageID::sStageEX)
        {
            // ボス戦なら描画しない
            return;
        }
    }

	UISprite_.SetPosition(UI_POS);
	UISprite_.SetScale(Vector3(0.25f, 0.25f, 0.25f));
	UISprite_.Update();
}

void TimerUI::Render(RenderContext& rc)
{
    // ボス戦時は描画しない。
    auto* pStageManager = app::core::StageManager::GetInstance();

    if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
        return;

	UISprite_.Draw(rc);
}


void TimerUI::Reset()
{
    UISprite_.SetPosition(UI_POS);
    UISprite_.SetScale(UI_SCALE);
    UISprite_.Update();
}
