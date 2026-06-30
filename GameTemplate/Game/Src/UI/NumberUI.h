#pragma once

#include <string>

#include "Src/UI/UIBase.h"
#include "stdint.h"

/**
 * @file   NumberUI.h
 * @brief  制限時間の数字表示 UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class NumberUI
         * @brief 残り秒数を表示し、0 で GameOver へ遷移するシングルトン GO。
         */
        class NumberUI : public UIBase
        {
        public:
            NumberUI() = default;
            virtual ~NumberUI() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            const std::string InitUI(const std::string& UIname) override { return UIBase::InitUI(UIname); }

            /** @brief 残り時間をカウントダウンする。 */
            void UpdateTimer();

            /** @brief フォントの初期設定。 */
            void InitUINumber();

            /** @return タイマー開始済みか。 */
            inline bool IsTimerStart() const { return isTimerStart_; }

            /** @return 残り秒数。 */
            inline float GetTimer() const { return timer_; }

            /** @brief 各ステージ開始時に 90 秒へリセット。 */
            inline void ResetTimer() { timer_ = 90.0f; }

            /** @return シングルトンインスタンス。 */
            inline static NumberUI* GetInstance() { return instance_; }

        private:
            bool isTimeUIDrawing_ = true; //!< 描画フラグ（予約）。
            bool isTimerStart_ = false;   //!< タイマー開始フラグ。
            bool timeUpFlag_ = false;     //!< タイムアップ遷移済みフラグ。
            float timer_ = 90.0f;         //!< 残り秒数。
            float colorChange_ = 0.0f;    //!< 色変化用（予約）。

            FontRender timerFont_;  //!< 秒数フォント。
            wchar_t timerText_[32]; //!< 表示バッファ。

            static NumberUI* instance_; //!< シングルトン。
        };
    } // namespace nsUI
} // namespace nsApp

using NumberUI = nsApp::nsUI::NumberUI;

namespace app
{
    namespace nsUI
    {
        using NumberUI = nsApp::nsUI::NumberUI;
    } // namespace nsUI
} // namespace app
