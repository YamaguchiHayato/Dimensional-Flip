#include "stdafx.h"

#include "TutorialMessageUI.h"

namespace nsApp
{
    namespace nsUI
    {
        bool TutorialMessageUI::Start()
        {
            frameSprite_.Init("Assets/UI/frame.DDS", 700, 500);
            frameSprite_.SetPivot({0.5f, 0.5f});
            frameSprite_.SetScale(Vector3(0.5f, 0.5f, 0.0f));
            frameSprite_.SetPosition(Vector3(200.0f, 0.0f, 0.0f));

            messageFont_.SetColor(Vector4::Black);

            isVisible_ = true;
            currentState_ = TutorialMessageState::None;
            return true;
        }

        void TutorialMessageUI::Update()
        {
            computedScreenPos_ = Vector3(800.0f, 320.0f, 0.0f);

            frameSprite_.SetPosition(computedScreenPos_);
            messageFont_.SetPosition(Vector3(computedScreenPos_.x - 170.0f, computedScreenPos_.y, 0.0f));

            if (currentState_ == TutorialMessageState::Complete)
            {
                completeFlashTimer_ += g_gameTime->GetFrameDeltaTime();

                if (completeFlashTimer_ > 0.5f)
                {
                    NextMessageStep();
                    completeFlashTimer_ = 0.0f;
                }
            }

            frameSprite_.Update();
        }

        void TutorialMessageUI::Render(RenderContext& rc)
        {
            if (!isVisible_)
                return;

            frameSprite_.Draw(rc);

            if (currentState_ != TutorialMessageState::None)
                messageFont_.Draw(rc);
        }

        void TutorialMessageUI::SetMessageList(const std::vector<std::wstring>& message)
        {
            messageList_ = message;
            currentIndex_ = 0;
            UpdateCurrentMessage();
            currentState_ = TutorialMessageState::Todo;
        }

        bool TutorialMessageUI::NextMessageStep()
        {
            currentIndex_++;
            if (currentIndex_ < messageList_.size())
            {
                UpdateCurrentMessage();
                currentState_ = TutorialMessageState::Todo;
                return true;
            }

            return false;
        }

        void TutorialMessageUI::ShowAt(const Vector3& worldPosition)
        {
            isVisible_ = true;
            targetWorldPos_ = worldPosition;
            isWorldPosMode_ = true;
        }
    } // namespace nsUI
} // namespace nsApp
