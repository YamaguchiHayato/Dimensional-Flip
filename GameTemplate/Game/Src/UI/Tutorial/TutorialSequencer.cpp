#include "stdafx.h"

#include "Src/UI/Tutorial/TutorialCompleteUI.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"
#include "TutorialSequencer.h"

namespace nsApp
{
    namespace nsUI
    {
        bool TutorialSequencer::Start()
        {
            pTutorialMessageUI_ = FindGO<TutorialMessageUI>("TutorialMessageUI");
            pTutorialCompleteUI_ = FindGO<TutorialCompleteUI>("TutorialCompleteUI");

            RefreshMessageList();

            return true;
        }

        void TutorialSequencer::RefreshMessageList()
        {
            if (!pTutorialMessageUI_)
                pTutorialMessageUI_ = FindGO<TutorialMessageUI>("TutorialMessageUI");

            if (!pTutorialMessageUI_ || steps_.empty())
                return;

            std::vector<std::wstring> messageList;
            for (auto& s : steps_)
                messageList.push_back(s.currentMassage);

            pTutorialMessageUI_->SetMessageList(messageList);
        }

        void TutorialSequencer::Update()
        {
            if (isAllFinished_ || !pTutorialMessageUI_)
                return;

            if (pTutorialMessageUI_->IsBusy())
                return;

            if (currentStepIndex_ >= steps_.size())
            {
                isAllFinished_ = true;
                return;
            }

            if (steps_[currentStepIndex_].condition())
            {
                pTutorialMessageUI_->NotifyComplete();

                if (pTutorialCompleteUI_)
                    pTutorialCompleteUI_->Play();

                currentStepIndex_++;
            }
        }

        void TutorialSequencer::Render(RenderContext& rc) {}
    } // namespace nsUI
} // namespace nsApp
