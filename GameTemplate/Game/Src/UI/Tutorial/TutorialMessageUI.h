#pragma once

#include <string>
#include <vector>

#include "Src/UI/UIBase.h"

/**
 * @file   TutorialMessageUI.h
 * @brief  チュートリアル用メッセージ枠 UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum TutorialMessageState
         * @brief メッセージ表示の状態。
         */
        enum class TutorialMessageState : uint8_t
        {
            None,     //!< 非表示
            Todo,     //!< 条件達成待ち
            Complete, //!< 達成演出中（短時間後に次へ）
        };

        /**
         * @class TutorialMessageUI
         * @brief チュートリアル手順のテキストを枠付きで表示する GO。
         */
        class TutorialMessageUI : public UIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TutorialMessageUI() = default;
            virtual ~TutorialMessageUI() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            /**
             * @brief 表示テキストを1件設定する。
             * @param message 表示文字列。nullptr なら何もしない。
             */
            void SetMessage(const wchar_t* message)
            {
                if (message == nullptr)
                    return;
                wcscpy_s(currentMessage_, 256, message);
                messageFont_.SetText(currentMessage_);
            }

            /**
             * @brief 条件達成時に Complete 演出へ遷移する。
             */
            void NotifyComplete()
            {
                if (currentState_ == TutorialMessageState::Todo)
                {
                    currentState_ = TutorialMessageState::Complete;
                    completeFlashTimer_ = 0.0f;
                }
            }

            /**
             * @brief 手順メッセージ一覧を設定し Todo 状態で開始する。
             * @param message メッセージリスト。
             */
            void SetMessageList(const std::vector<std::wstring>& message);

            /**
             * @brief 次のメッセージへ進める。
             * @return 次があれば true。
             */
            bool NextMessageStep();

            /**
             * @brief 現在インデックスのメッセージをフォントに反映する。
             */
            void UpdateCurrentMessage()
            {
                if (currentIndex_ < messageList_.size())
                {
                    wcscpy_s(currentMessage_, 256, messageList_[currentIndex_].c_str());
                    messageFont_.SetText(currentMessage_);
                }
            }

            /**
             * @brief ワールド座標基準で表示する（3D モード用）。
             * @param worldPosition 基準ワールド座標。
             */
            void ShowAt(const Vector3& worldPosition);

            /** @brief 非表示にする。 */
            void Hide() { isVisible_ = false; }

            inline bool IsVisible() const { return isVisible_; }

            /**
             * @brief Complete 演出中かどうか。
             * @return 演出中なら true（Sequencer は待機する）。
             */
            inline bool IsBusy() const { return currentState_ == TutorialMessageState::Complete; }

        private:
            std::vector<std::wstring> messageList_;  //!< 手順メッセージ一覧。
            TutorialMessageState currentState_;      //!< 現在の表示状態。
            SpriteRender frameSprite_;               //!< 枠スプライト。
            FontRender messageFont_;                 //!< メッセージフォント。
            Vector3 computedScreenPos_;              //!< 計算済みスクリーン座標。
            Vector3 targetWorldPos_ = Vector3::Zero; //!< 追従ワールド座標。
            Vector3 screenPos_ = Vector3::Zero;      //!< スクリーン座標（予約）。
            Matrix worldMatrix_;                     //!< ワールド行列（予約）。
            wchar_t currentMessage_[256];            //!< 現在表示テキスト。
            float completeFlashTimer_ = 0.0f;        //!< Complete 演出タイマー。
            int currentIndex_ = 0;                   //!< 現在のメッセージ index。
            bool isVisible_ = false;                 //!< 表示中か。
            bool isWorldPosMode_ = false;            //!< ワールド追従モードか。
        };
    } // namespace nsUI
} // namespace nsApp

using TutorialMessageState = nsApp::nsUI::TutorialMessageState;
using TutorialMessageUI = nsApp::nsUI::TutorialMessageUI;

namespace app
{
    namespace nsUI
    {
        using TutorialMessageState = nsApp::nsUI::TutorialMessageState;
        using TutorialMessageUI = nsApp::nsUI::TutorialMessageUI;
    } // namespace nsUI
} // namespace app
