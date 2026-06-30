#include "stdafx.h"

#include "NumberUI.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"

namespace
{
    const Vector4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
    const Vector4 RED(1.0f, 0.0f, 0.0f, 1.0f);
    const Vector3 TIMER_POS{-940.0f, 430.0f, 0.0f};
    const float TIMER_SCALE = 2.0f;
    const float WARNING_TIME = 30.0f;
} // namespace

namespace nsApp
{
    namespace nsUI
    {
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
            UpdateTimer();

            swprintf_s(timerText_, L"%02d", static_cast<int>(timer_));
            timerFont_.SetText(timerText_);
            timerFont_.SetColor(timer_ <= WARNING_TIME ? RED : WHITE);
        }

        void NumberUI::Render(RenderContext& rc)
        {
            auto* pStageManager = nsStage::StageManager::GetInstance();
            if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
                return;

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
            auto* pStageManager = nsStage::StageManager::GetInstance();
            if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
                return;

            if (timeUpFlag_)
                return;

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
    } // namespace nsUI
} // namespace nsApp
