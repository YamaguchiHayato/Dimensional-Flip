#pragma once
#include "Src/UI/UIBase.h"
#include <string>
#include <vector>

namespace app
{
    namespace nsUI
    {
        // メッセージの状態。
        enum class TutorialMessageState : uint8_t
        {
            None = 0, // ディアクティブ状態。
            Todo,     // やることメッセージ(条件達成待ち)。
            Complete, // 完了メッセージ(条件達成後,数秒後にフェード)
        };


        class TutorialMessageUI : public UIBase
        {
        public:
            TutorialMessageUI() = default;
            virtual ~TutorialMessageUI() = default;


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        // メッセージ処理。
        public:
            // メッセージを設定
            void SetMessage(const wchar_t* message)
            {
                if (message == nullptr)
                    return;

                wcscpy_s(currentMessage_, 256, message);

                messageFont_.SetText(currentMessage_);
            }

            // 目的を達成した時に呼び出す。
            void NotifyComplete()
            {
                if (currentState_ == TutorialMessageState::Todo)
                {
                    currentState_ = TutorialMessageState::Complete;
                    completeFlashTimer_ = 0.0f; // ヘッダにある変数名に合わせる
                }
            }

            // メッセージの種類を格納するリスト。
            void SetMessageList(const std::vector<std::wstring>& message);

            // 次のメッセージへ進める。
            bool NextMessageStep();

            //
            void UpdateCurrentMessage()
            {
                if (currentIndex_ < messageList_.size())
                {
                    wcscpy_s(currentMessage_, 256, messageList_[currentIndex_].c_str());
                    messageFont_.SetText(currentMessage_);
                }
            }


            // プレイヤー座標に基づいて表示
            void ShowAt(const Vector3& worldPosition);

            // 非表示
            void Hide()
            {
                isVisible_ = false;
            }

            inline bool IsVisible() const
            {
                return isVisible_;
            }

            // 現在演出中か確認。
            inline bool IsBusy() const
            {
                return currentState_ == TutorialMessageState::Complete;
            }

        private:
            std::vector<std::wstring> messageList_; // メッセージリスト
            TutorialMessageState currentState_;
            SpriteRender frameSprite_;
            FontRender messageFont_;
            Vector3 computedScreenPos_;
            Vector3 targetWorldPos_ = Vector3::Zero;
            Vector3 screenPos_ = Vector3::Zero;

            Matrix worldMatrix_;

            wchar_t currentMessage_[256];

            float completeFlashTimer_ = 0.0f;

            int currentIndex_ = 0;

            bool isVisible_ = false;
            bool isWorldPosMode_ = false; // 今が3Dmodeか判断。

        };
    } // namespace nsUI
} // namespace app
