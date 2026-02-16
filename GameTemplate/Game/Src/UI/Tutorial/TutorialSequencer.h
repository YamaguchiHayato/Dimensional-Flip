#pragma once
// TutorialStageの文字を切り替えるクラス。

namespace app
{
    namespace nsUI
    {
        class TutorialMessageUI;
        class TutorialCompleteUI;

        // 描画する文字のステップを管理する構造体。
        struct TutorialStep
        {
            std::wstring currentMassage; // 現在のメッセージ。
            std::function<bool()> condition; // trueを返すとクリア。
        };


        class TutorialSequencer : public IGameObject
        {
        public:
            TutorialSequencer() = default;
            virtual ~TutorialSequencer() = default;

        public:
            bool Start();
            void Update();
            void Render(RenderContext& rc);

        public:
            // 手順を追加する。
            inline void AddStep(const std::wstring& message, std::function<bool()> command)
            {
                steps_.push_back({message, command});
            }


        // ゲッター。
        public:
            // 全てのステップが終了しているか。
            inline bool IsAllFinished() const
            {
                return isAllFinished_;
            }

        private:
            TutorialMessageUI* pTutorialMessageUI_ = nullptr;
            TutorialCompleteUI* pTutorialCompleteUI_ = nullptr;

        private:
            std::vector<TutorialStep> steps_;
            FontRender tutorialMessage_; 

            int currentStepIndex_ = 0;
            bool isAllFinished_ = false;

        };

    }
}
