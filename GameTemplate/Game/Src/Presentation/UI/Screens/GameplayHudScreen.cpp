#include "stdafx.h"

#include "GameplayHudScreen.h"
#include "Src/Core/Game.h"
#include "Src/Core/StageManager.h"
#include "Src/Presentation/Data/IGameplayHubData.h" 
#include "Src/Presentation/Data/GameplayHudData.h"

namespace nsApp
{
    namespace nsUI
    {
        GameplayHudScreen::GameplayHudScreen()
        {
            SetupLayout();
        }


        void GameplayHudScreen::SetDataSource(nsPresentation::IGameplayHudData* pDataSource)
        {
            pDataSource_ = pDataSource;
        }


        void GameplayHudScreen::SetupLayout()
        {
            /* 旧 NumberUI / ScoreUI / HPbarUI と同じ付近 */
            timerView_.SetPosition(-940.0f, 430.0f);
            timerView_.SetDigitSpacing(24.0f);

            scoreView_.SetPosition(625.0f, 550.0f);
            scoreView_.SetDigitSpacing(20.0f);

            hpBarView_.SetPosition(-900.0f, 500.0f);
            hpBarLogic_.SetBarWidth(252.0f);
        }


        void GameplayHudScreen::ConnectToData(nsPresentation::GameplayHudData* pData)
        {
            if (pData == nullptr)
                return;

            SetVisible(true);
            pData->SetScreen(this);
            SetDataSource(pData);
            SyncFromDataSource();
        }


        void GameplayHudScreen::SetScoreVisible(bool visible)
        {
            scoreView_.SetVisible(visible);
        }


        void GameplayHudScreen::Build()
        {
            /* UIScreenHost::Start が Build() を呼ぶ。Entity ツリーは今は空で OK */
            CreateRootEntity();
        }


        void GameplayHudScreen::SyncFromDataSource()
        {
            if (pDataSource_ == nullptr)
                return;

            /* Data → Logic */
            timerLogic_.SetSeconds(pDataSource_->GetTimerSeconds());
            scoreLogic_.SetScore(pDataSource_->GetScore());
            hpBarLogic_.SetHpRatio(pDataSource_->GetPlayerHpRatio());

            /* Logic → View */
            timerView_.ApplyLogic(timerLogic_);
            scoreView_.ApplyLogic(scoreLogic_);
            hpBarView_.ApplyLogic(hpBarLogic_);
        }


        void GameplayHudScreen::DrawHud(RenderContext& rc)
        {
            // フェードイン完了まで HUD を描画しない（Game::Update と揃える）
            if (auto* pFade = SceneManager::GetInstance()->GetFade())
            {
                if (!pFade->IsFadeInEnd())
                    return;
            }

            if (!IsVisible())
                return;

            /* Game が無い（シーン遷移後など）なら描画しない */
            if (FindGO<nsCore::Game>("game") == nullptr)
                return;

            /* ボス戦ではインゲーム HUD を出さない（旧 UI と同じ） */
            auto* pStageManager = nsStage::StageManager::GetInstance();
            if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
                return;

            SyncFromDataSource();

            timerView_.Draw(rc);
            scoreView_.Draw(rc);
            hpBarView_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
