#pragma once

#include "Src/UI/UIBase.h"

/**
 * @file   TutorialCompleteUI.h
 * @brief  チュートリアル手順クリア時の「Complete」演出 UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class TutorialCompleteUI
         * @brief 下からスライドインする Complete テキストを表示する。
         */
        class TutorialCompleteUI : public UIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TutorialCompleteUI() = default;
            virtual ~TutorialCompleteUI() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            /**
             * @brief Complete スライド演出を開始する。
             */
            void Play();

            /**
             * @brief 演出が終了したかどうか。
             */
            inline bool IsFinished() const { return isFinished_; }

        private:
            FontRender completeFont_;                 //!< Complete フォント。
            Vector3 currentPosition_ = Vector3::Zero; //!< 現在描画位置。
            Vector3 targetPosition_ = Vector3::Zero;  //!< スライド目標位置。
            float timer_ = 0.0f;                      //!< 演出経過時間。
            bool isPlaying_ = false;                  //!< 再生中か。
            bool isFinished_ = false;                 //!< 終了済みか。
        };
    } // namespace nsUI
} // namespace nsApp

using TutorialCompleteUI = nsApp::nsUI::TutorialCompleteUI;

namespace app
{
    namespace nsUI
    {
        using TutorialCompleteUI = nsApp::nsUI::TutorialCompleteUI;
    }
} // namespace app
