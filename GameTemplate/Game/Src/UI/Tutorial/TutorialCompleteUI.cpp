#include "stdafx.h"

#include "TutorialCompleteUI.h"

namespace
{
    const float SLIDE_TIME = 0.5f;   // スライドにかかる時間
    const float DISPLAY_TIME = 1.5f; // 表示し続ける時間
} // namespace

namespace app
{
    namespace nsUI
    {
        bool TutorialCompleteUI::Start()
        {
            completeFont_.SetText(L"Complete");
            completeFont_.SetColor({0.6f, 0.0f, 0.4f, 1.0f});
            completeFont_.SetScale(1.2);

            float targetX = 650.0f;

            // 初期位置（画面外の下）
            currentPosition_ = Vector3(targetX, 1000.0f, 0.0f);

            // 目標位置（枠の下）
            targetPosition_ = Vector3(targetX, 400.0f, 0.0f);

            isPlaying_ = false;
            isFinished_ = false;

            return true;
        }

        void TutorialCompleteUI::Update()
        {
            if (!isPlaying_)
                return;

            timer_ += g_gameTime->GetFrameDeltaTime();

            // スライド中（タイマーがスライド時間以下のとき）
            if (timer_ <= SLIDE_TIME)
            {
                auto rate = timer_ / SLIDE_TIME;
                auto slideRate = 1.0f - pow(1.0f - rate, 3); // イージング

                currentPosition_.y = 1000.0f + (targetPosition_.y - 1000.0f) * slideRate;
            }

            // 待機時間経過後
            else if (timer_ > SLIDE_TIME + DISPLAY_TIME)
            {
                isPlaying_ = false;
                isFinished_ = true;
            }

            completeFont_.SetPosition(currentPosition_);
        }

        void TutorialCompleteUI::Play()
        {
            if (isPlaying_)
                return;

            isPlaying_ = true;
            isFinished_ = false;

            timer_ = 0.0f;

            // 再生開始時は確実に下に配置
            currentPosition_.y = 1000.0f;
        }

        void TutorialCompleteUI::Render(RenderContext& rc)
        {
            if (isPlaying_)
                completeFont_.Draw(rc);
        }
    } // namespace nsUI
} // namespace app
