#include "stdafx.h"
#include "NumberUI.h"


#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"

namespace
{
	const float UI_WIDTH = 300.0f;
	const float UI_HEIGHT = 300.0f;
	const Vector3 UI_POS{ 100.0f, 450.0f, 0.0f };

	const Vector4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	const Vector3 UI_SCALE(0.5f, 0.5f, 0.5f );
    const Vector3 TIMER_POS{-940.0f, 430.0f, 0.0f};
    const float TIMER_SCALE = 2.0f;

    const float WARNING_TIME = 30.0f; // この秒数以下で赤（好みで 10 など）
    const Vector4 RED(1.0f, 0.0f, 0.0f, 1.0f);
} // namespace
NumberUI* NumberUI::instance_ = nullptr;


bool NumberUI::Start()
{
    instance_ = this;
	InitUINumber();
	isTimerStart_ = true;
	return true;
}

void NumberUI::Update()
{
    // タイマーの更新処理。
	UpdateTimer();

    // 現在の秒数を2桁の文字列に変換。
    swprintf_s(timerText_, L"%02d", static_cast<int>(timer_));
    timerFont_.SetText(timerText_);

    if (timer_ <= WARNING_TIME)
        timerFont_.SetColor(RED);
    else
        timerFont_.SetColor(WHITE);
}

void NumberUI::Render(RenderContext& rc)
{
    auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
    if (pStageManager)
    {
        if (pStageManager->GetCurrentStageID() == StageID::sStageEX)
        {
            // ボス戦なら描画しない
            return;
        }
    }

    timerFont_.Draw(rc);
}


void NumberUI::InitUINumber()
{
    timerFont_.SetPosition(TIMER_POS);
    timerFont_.SetScale(TIMER_SCALE);
    timerFont_.SetColor(WHITE);
}


void NumberUI::UpdateTimer()
{
    auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
    if (pStageManager)
    {
        if (pStageManager->GetCurrentStageID() == StageID::sStageEX)
            return; // ボス戦はタイマーなし
    }

    if (timeUpFlag_)
        return; // 二重遷移防止

    if (timer_ > 0.0f)
    {
        timer_ -= g_gameTime->GetFrameDeltaTime();

        if (timer_ <= 0.0f)
        {
            timer_ = 0.0f;
            timeUpFlag_ = true;
            SceneManager::GetInstance()->ChangeScene(SceneID::sGameOver);
        }
    }
}
