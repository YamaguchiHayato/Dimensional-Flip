#include "stdafx.h"
#include "TutorialSequencer.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"
#include "Src/UI/Tutorial/TutorialCompleteUI.h"

namespace app
{
    namespace nsUI
    {
        bool TutorialSequencer::Start()
        {
            pTutorialMessageUI_ = FindGO<TutorialMessageUI>("TutorialMessageUI");

            pTutorialCompleteUI_ = FindGO<TutorialCompleteUI>("TutorialCompleteUI");

            // UIの初期設定。
            if (pTutorialMessageUI_ && !steps_.empty())
            {
                std::vector<std::wstring> messageList;
                for (auto& s : steps_)
                {
                    // メッセージリストに追加。
                    messageList.push_back(s.currentMassage);

                    // 最初のメッセージを設定。
                    pTutorialMessageUI_->SetMessageList(messageList);
                }
            }

            return true;
        }


        void TutorialSequencer::Update()
        {
            // UIが演出中でないか、UIが存在しない場合は処理しない。
            if (isAllFinished_ || !pTutorialMessageUI_)
                return;

            // UIが忙しい状態なら処理しない。
            if (pTutorialMessageUI_->IsBusy())
                return;

            // 全ステップが終了しているか確認。
            if (currentStepIndex_ >= steps_.size())
            {
                isAllFinished_ = true;
                return;
            }

            // UIが TODO 状態かを確認。
            // 現在のステップに満たしているかを判断。
            if (steps_[currentStepIndex_].condition())
            {
                // Completeへ遷移。
                pTutorialMessageUI_->NotifyComplete();

                // Completeの演出を再生。
                if (pTutorialMessageUI_)
                    pTutorialCompleteUI_->Play();

                // 次のステップへ。
                currentStepIndex_++;
            }
        }


        void TutorialSequencer::Render(RenderContext& rc)
        {

        }
    }
}
