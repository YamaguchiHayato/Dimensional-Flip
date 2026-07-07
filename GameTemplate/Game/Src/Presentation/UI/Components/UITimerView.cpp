#include "stdafx.h"

#include "Src/Presentation/UI/Components/UITimerView.h"
#include "Src/Presentation/UI/Logic/TImeDisplayLogic.h"

namespace
{
    // 旧 NumberUI
    const Vector4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
    const Vector4 RED(1.0f, 0.0f, 0.0f, 1.0f);
    const float TIMER_SCALE = 2.0f;
    const float WARNING_TIME = 30.0f;

    // 旧 TimerUI（背景バー）
    const auto TIMER_BAR_WIDTH = 1210.0f;
    const auto TIMER_BAR_HEIGHT = 720.0f;
    const Vector3 TIMER_BAR_POS{-870.0f, 360.0f, 0.0f};
    const Vector3 TIMER_BAR_SCALE(0.25f, 0.25f, 0.25f);
    const char* kTimerBarPath = "Assets/UI/progressBar/Time.DDS";
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        UITimerView::UITimerView()
            : posX_(0.0f), posY_(0.0f), digitSpacing_(24.0f), cachedSeconds_(0), timerBarInitialized_(false)
        {
            InitFont();
            InitTimerBar();
        }

        void UITimerView::InitFont()
        {
            timerFont_.SetScale(TIMER_SCALE);
            timerFont_.SetColor(WHITE);
        }

        void UITimerView::InitTimerBar()
        {
            // 旧 TimerUI::Start と同じ
            timerBar_.Init(kTimerBarPath, TIMER_BAR_WIDTH, TIMER_BAR_HEIGHT);
            timerBar_.SetPosition(TIMER_BAR_POS);
            timerBar_.SetScale(TIMER_BAR_SCALE);
            timerBar_.Update();
            timerBarInitialized_ = true;
        }

        void UITimerView::SetPosition(float x, float y)
        {
            posX_ = x;
            posY_ = y;
            timerFont_.SetPosition(Vector3(posX_, posY_, 0.0f));
        }

        void UITimerView::SetDigitSpacing(float spacing)
        {
            digitSpacing_ = spacing;
        }

        void UITimerView::ApplyLogic(const TimerDisplayLogic& logic)
        {
            cachedSeconds_ = logic.GetSeconds();

            swprintf_s(timerText_, L"%02d", cachedSeconds_);
            timerFont_.SetText(timerText_);
            timerFont_.SetColor(cachedSeconds_ <= static_cast<int>(WARNING_TIME) ? RED : WHITE);
            timerFont_.SetPosition(Vector3(posX_, posY_, 0.0f));

            // 背景バーも毎フレーム更新（旧 TimerUI::Update と同じ）
            if (timerBarInitialized_)
            {
                timerBar_.SetPosition(TIMER_BAR_POS);
                timerBar_.SetScale(TIMER_BAR_SCALE);
                timerBar_.Update();
            }
        }

        void UITimerView::Draw(RenderContext& rc)
        {
            // 背景 → 数字の順（旧 TimerUI + NumberUI）
            if (timerBarInitialized_)
                timerBar_.Draw(rc);

            timerFont_.Draw(rc);
        }
    } /* namespace nsUI */
} /* namespace nsApp */
