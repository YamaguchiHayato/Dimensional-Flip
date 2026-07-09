#include "stdafx.h"

#include "PauseController.h"
#include "Src/Actor/Stage/StageSetup.h"
#include "Src/Core/Game.h"
#include "Src/Production/Fade.h"
#include "Src/UI/Pause/PauseMenuUI.h"

namespace nsApp
{
    namespace nsCore
    {
        bool PauseController::IsPauseTogglePressed() const
        {
            /* Xbox 中央右側(Menu)を想定: enButtonStart。既存入力に合わせて Select も許可。 */
            return g_pad[0]->IsTrigger(enButtonStart) || g_pad[0]->IsTrigger(enButtonSelect);
        }

        bool PauseController::CanOpen(Game* pGame) const
        {
            /* Game が無いなら開けない。 */
            if (pGame == nullptr)
                return false;

            /* ステージ遷移中は開かない。 */
            if (pGame->IsStageTransitioning())
                return false;

            /* フェードイン中は開かない。 */
            auto* pFade = SceneManager::GetInstance()->GetFade();
            if (pFade && !pFade->IsFadeInEnd())
                return false;

            /* ボスカットインなど、ステージ側で停止中なら開かない。 */
            if (nsStage::StageSetup::ShouldKeepPlayerPaused())
                return false;

            return true;
        }

        void PauseController::Open()
        {
            /* ポーズ中にする。 */
            isPaused_ = true;
            selectedIndex_ = 0;

            /* ポーズメニューを開く。 */
            if (pPauseMenu_)
            {
                pPauseMenu_->Open();
                pPauseMenu_->SetSelectedIndex(selectedIndex_);
            }

            /* ゲーム時間を固定する。 */
            g_gameTime->EnableFixedFrameDeltaTime(0.0f);
        }

        void PauseController::Close()
        {
            /* ポーズを解除する。 */
            isPaused_ = false;

            /* ポーズメニューを閉じる。 */
            if (pPauseMenu_)
                pPauseMenu_->Close();

            /* ゲーム時間の固定を解除する。 */
            g_gameTime->DisableFixedFrameDeltaTime();
        }

        PauseAction PauseController::Update(Game* pGame)
        {
            /* ポーズトグルが押されたかを確認する。 */
            const bool toggle = IsPauseTogglePressed();

            /* ポーズ中でない場合、トグルが押されていて開けるなら開く。 */
            if (!isPaused_)
            {
#ifdef _DEBUG
                /* デバッグ時のみ B でも開く。 */
                if (g_pad[0]->IsTrigger(enButtonB) && CanOpen(pGame))
                {
                    Open();
                    return PauseAction::None;
                }
#endif
                if (toggle && CanOpen(pGame))
                    Open();

                return PauseAction::None;
            }

            /* 上下で項目を切り替える。 */
            if (g_pad[0]->IsTrigger(enButtonUp))
            {
                selectedIndex_ = (selectedIndex_ - 1 + kItemCount) % kItemCount;
                if (pPauseMenu_)
                    pPauseMenu_->SetSelectedIndex(selectedIndex_);
            }

            if (g_pad[0]->IsTrigger(enButtonDown))
            {
                selectedIndex_ = (selectedIndex_ + 1) % kItemCount;
                if (pPauseMenu_)
                    pPauseMenu_->SetSelectedIndex(selectedIndex_);
            }

            /* B / Menu はポーズ解除。 */
            if (g_pad[0]->IsTrigger(enButtonB) || toggle)
            {
                Close();
                return PauseAction::Resume;
            }

            /* A で選択中項目を決定する。 */
            if (g_pad[0]->IsTrigger(enButtonA))
            {
                const int decided = selectedIndex_;

                /* Setting はポーズ継続のため、完全 Close せずメニューだけ隠す。 */
                if (decided == 2)
                {
                    if (pPauseMenu_)
                        pPauseMenu_->Close();
                    return PauseAction::SoundSetting;
                }

                /* Restart / ReWorld は通常どおりポーズ解除する。 */
                Close();

                if (decided == 0)
                    return PauseAction::RestartStage;

                return PauseAction::BackToWorldSelect;
            }

            return PauseAction::None;
        }
    } // namespace nsCore
} // namespace nsApp
