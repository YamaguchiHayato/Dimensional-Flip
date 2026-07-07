#include "stdafx.h"

#include "GameplayTimerHudComponent.h"

namespace
{
    const Vector4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
    const Vector4 RED(1.0f, 0.0f, 0.0f, 1.0f);
    const float TIMER_SCALE = 2.0f;
    const float WARNING_TIME = 30.0f;

    const Vector3 TIMER_BAR_POS{-870.0f, 360.0f, 0.0f};
    const Vector3 TIMER_BAR_SCALE(0.25f, 0.25f, 0.25f);
    const char* kTimerBarPath = "Assets/UI/progressBar/Time.DDS";
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void GameplayTimerHudComponent::SetTextPosition(float x, float y)
        {
            /* タイマーのテキスト表示位置を設定する。*/
            textPosX_ = x;
            textPosY_ = y;
        }


        void GameplayTimerHudComponent::OnBuild()
        {
            /* タイマーのテキスト表示を初期化する。*/
            timerFont_.SetScale(TIMER_SCALE);
            timerFont_.SetColor(WHITE);

            /* タイマーバーのスプライトを初期化する。*/
            timerBar_.Init(kTimerBarPath, 1210.0f, 720.0f);
            timerBar_.SetPosition(TIMER_BAR_POS);
            timerBar_.SetScale(TIMER_BAR_SCALE);
            timerBar_.Update();
            timerBarInitialized_ = true;
        }


        void GameplayTimerHudComponent::ApplyLogic(const TimerDisplayLogic& logic)
        {
            /* タイマーの残り時間を取得し、テキスト表示を更新する。*/
            cachedSeconds_ = logic.GetSeconds();
            swprintf_s(timerText_, L"%02d", cachedSeconds_);
            timerFont_.SetText(timerText_);
            timerFont_.SetColor(cachedSeconds_ <= static_cast<int>(WARNING_TIME) ? RED : WHITE);
            timerFont_.SetPosition(Vector3(textPosX_, textPosY_, 0.0f));
        }


        void GameplayTimerHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            /* 親 Entity のワールド行列は使用しない。*/
            (void) ownerWorld;

            /* タイマーのテキストを描画する。*/
            if (timerBarInitialized_)
            {
                timerBar_.SetPosition(TIMER_BAR_POS);
                timerBar_.SetScale(TIMER_BAR_SCALE);
                timerBar_.Update();
                timerBar_.Draw(rc);
            }

            timerFont_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
