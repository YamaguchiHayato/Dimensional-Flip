#pragma once

#include <functional>
#include <string>
#include <vector>

/**
 * @file   TutorialSequencer.h
 * @brief  チュートリアル手順の進行を管理する GO。
 */

namespace nsApp
{
    namespace nsUI
    {
        class TutorialMessageUI;
        class TutorialCompleteUI;

        /**
         * @struct TutorialStep
         * @brief 1 手順分のメッセージとクリア条件。
         */
        struct TutorialStep
        {
            std::wstring currentMassage;     //!< 表示メッセージ。
            std::function<bool()> condition; //!< true でクリア。
        };

        /**
         * @class TutorialSequencer
         * @brief 手順を順に進め、MessageUI / CompleteUI と連携する。
         */
        class TutorialSequencer : public IGameObject
        {
        public:
            TutorialSequencer() = default;
            virtual ~TutorialSequencer() = default;

            bool Start();
            void Update();
            void Render(RenderContext& rc);

            inline void AddStep(const std::wstring& message, std::function<bool()> command)
            {
                steps_.push_back({message, command});
            }

            void RefreshMessageList();

            inline bool IsAllFinished() const { return isAllFinished_; }

        private:
            TutorialMessageUI* pTutorialMessageUI_ = nullptr;
            TutorialCompleteUI* pTutorialCompleteUI_ = nullptr;

            std::vector<TutorialStep> steps_;
            FontRender tutorialMessage_;

            int currentStepIndex_ = 0;
            bool isAllFinished_ = false;
        };
    } // namespace nsUI
} // namespace nsApp

using TutorialStep = nsApp::nsUI::TutorialStep;
using TutorialSequencer = nsApp::nsUI::TutorialSequencer;

namespace app
{
    namespace nsUI
    {
        using TutorialStep = nsApp::nsUI::TutorialStep;
        using TutorialSequencer = nsApp::nsUI::TutorialSequencer;
    } // namespace nsUI
} // namespace app
