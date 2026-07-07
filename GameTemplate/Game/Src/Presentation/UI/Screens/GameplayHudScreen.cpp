#include "stdafx.h"
#include "Src/Actor/Character/Player/Player.h"  
#include "GameplayHudScreen.h"
#include "Src/Core/Game.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Presentation/Data/GameplayHudData.h"
#include "Src/Presentation/Data/IGameplayHubData.h"
#include "Src/Presentation/UI/Components/GameplayHpHudComponent.h"
#include "Src/Presentation/UI/Components/GameplayScoreHudComponent.h"
#include "Src/Presentation/UI/Components/GameplayTimerHudComponent.h"
#include "Src/Presentation/UI/Widget/UIPanel.h"

namespace nsApp
{
    namespace nsUI
    {
        void GameplayHudScreen::SetDataSource(nsPresentation::IGameplayHudData* pDataSource)
        {
            pDataSource_ = pDataSource;
        }


        void GameplayHudScreen::Build()
        {
            nsFramework::Entity& root = CreateRootEntity();
            canvas_.Initialize(root);

            pTimerComponent_ = &root.AddComponent<GameplayTimerHudComponent>();
            pTimerComponent_->SetTextPosition(-940.0f, 430.0f);

            pHpComponent_ = &root.AddComponent<GameplayHpHudComponent>();
            pHpComponent_->SetHeartPosition(-900.0f, 500.0f);

            /** @brief Timer/HP と同じ root 直下。子 Entity + SetEnabled は使わない。 */
            pScoreComponent_ = &root.AddComponent<GameplayScoreHudComponent>();
            pScoreComponent_->SetNumberPosition(625.0f, 550.0f);

            hpBarLogic_.SetBarWidth(252.0f);
        }


        void GameplayHudScreen::ConnectToData(nsPresentation::GameplayHudData* pData)
        {
            if (pData == nullptr)
                return;

            SetVisible(true);
            SetScoreVisible(true);
            pData->SetScreen(this);
            SetDataSource(pData);
            SyncFromDataSource();
        }


        void GameplayHudScreen::SetScoreVisible(bool visible)
        {
            if (pScorePanelEntity_ != nullptr)
                pScorePanelEntity_->SetEnabled(visible);
        }


        void GameplayHudScreen::SyncFromDataSource()
        {
            if (pDataSource_ == nullptr)
                return;

            timerLogic_.SetSeconds(pDataSource_->GetTimerSeconds());
            scoreLogic_.SetScore(pDataSource_->GetScore());
            hpBarLogic_.SetHpRatio(pDataSource_->GetPlayerHpRatio());

            if (pTimerComponent_ != nullptr)
                pTimerComponent_->ApplyLogic(timerLogic_);
            if (pScoreComponent_ != nullptr)
                pScoreComponent_->ApplyLogic(scoreLogic_);
        }


        bool GameplayHudScreen::CanDrawHud() const
        {
            if (!IsVisible())
                return false;

            if (auto* pFade = SceneManager::GetInstance()->GetFade())
            {
                if (!pFade->IsFadeInEnd())
                    return false;
            }

            if (FindGO<nsCore::Game>("game") == nullptr)
                return false;

            auto* pStageManager = nsStage::StageManager::GetInstance();
            if (pStageManager && pStageManager->GetCurrentStageID() == StageID::sStageEX)
                return false;

            return true;
        }


        void GameplayHudScreen::Draw(RenderContext& rc)
        {
            if (!CanDrawHud())
                return;

             /**
             * @brief チュートリアル完了後は毎フレームスコアを表示に戻す。
             *        Enter/Exit のペアが崩れた場合の保険。
             */
            if (auto* pPlayer = FindGO<nsApp::nsActor::nsCharacter::nsPlayer::Player>("player"))
            {
                if (pPlayer->IsTutorialDone())
                    SetScoreVisible(true);
            }

            SyncFromDataSource();
            GetRootEntity().Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
