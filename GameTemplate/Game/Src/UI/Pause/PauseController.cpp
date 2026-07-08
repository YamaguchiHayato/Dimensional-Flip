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
            // Xbox中央右側(Menu)を想定: enButtonStart
            // 既存入力資産に合わせて Select も許可
            bool pressed = g_pad[0]->IsTrigger(enButtonStart) || g_pad[0]->IsTrigger(enButtonSelect);

#ifdef _DEBUG
            // デバッグ時のみ B でもトグル可能
            pressed = pressed || g_pad[0]->IsTrigger(enButtonB);
#endif
            return pressed;
        }

        bool PauseController::CanOpen(Game* pGame) const
        {
            /* Gameクラスが無いならスキップ。*/
            if (pGame == nullptr)
                return false;

            /* ステージ遷移中は開かない。*/
            if (pGame->IsStageTransitioning())
                return false;

            /* フェードイン中は開かない。*/
            auto* pFade = SceneManager::GetInstance()->GetFade();
            if (pFade && !pFade->IsFadeInEnd())
                return false;

            /* ステージ設定でプレイヤーを止めるべきなら開かない。*/
            if (nsStage::StageSetup::ShouldKeepPlayerPaused())
                return false;

            return true;
        }


        void PauseController::Open()
        {
            /* ポーズ中にする。*/
            isPaused_ = true;

            /* ポーズメニューを開く。*/
            if (pPauseMenu_)
                pPauseMenu_->Open();

            /* ゲーム時間を固定する。*/
            g_gameTime->EnableFixedFrameDeltaTime(0.0f);
        }


        void PauseController::Close()
        {
            /* ポーズを解除する。*/
            isPaused_ = false;

            /* ポーズメニューを閉じる。*/
            if (pPauseMenu_)
                pPauseMenu_->Close();

            /* ゲーム時間の固定を解除する。*/
            g_gameTime->DisableFixedFrameDeltaTime();
        }


        PauseAction PauseController::Update(Game* pGame)
        {
            /* ポーズトグルが押されたかを確認する。*/
            const bool toggle = IsPauseTogglePressed();

            /* ポーズ中でない場合、トグルが押されていて開けるなら開く。*/
            if (!isPaused_)
            {
                /* ポーズ中でない場合、トグルが押されていて開けるなら開く。*/
                if (toggle && CanOpen(pGame))
                    Open();

                /* ポーズ中でない場合、トグルが押されていないか、開けない場合は何もしない。*/
                return PauseAction::None;
            }

            /* ポーズ中の場合、トグルが押されているか、Aボタンが押されている場合は閉じる。*/
            if (toggle || g_pad[0]->IsTrigger(enButtonA))
            {
                /* ポーズを解除する。*/
                Close();
                return PauseAction::Resume;
            }

            /* ポーズ中の場合、Bボタンが押されている場合はタイトルに戻る。*/
            if (g_pad[0]->IsTrigger(enButtonB))
            {
                /* ポーズを解除する。*/
                Close();
                return PauseAction::BackToTitle;
            }

            return PauseAction::None;
        }
    } // namespace nsCore
} // namespace nsApp
