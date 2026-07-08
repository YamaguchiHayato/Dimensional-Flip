#include "stdafx.h"

#include "GameplayHudScreen.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/Game.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Presentation/Data/GameplayHudData.h"
#include "Src/Presentation/Data/IGameplayHubData.h"
#include "Src/Presentation/UI/Components/GameplayHpHudComponent.h"
#include "Src/Presentation/UI/Components/GameplayScoreHudComponent.h"
#include "Src/Presentation/UI/Components/GameplayTimerHudComponent.h"
#include "Src/Presentation/UI/Widget/UIPanel.h"
#include "Src/Actor/Stage/StageSetup.h"

namespace
{
    /**
     * @brief 現在ステージがボス戦かどうか。
     * @return ボス戦なら true。
     */
    bool IsBossStage()
    {
        auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
        if (pStageManager == nullptr)
            return false;

        return pStageManager->GetCurrentStageID() == nsApp::nsStage::StageID::sStageEX;
    }

    /**
     * @brief ボス戦のカットインが再生中かどうか。
     * @return 再生中なら true。
     */
    bool IsBossCutInPlaying()
    {
        return nsApp::nsStage::StageSetup::ShouldKeepPlayerPaused();
    }
} // namespace

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
            /* スコア Component へ直接委譲する。 */
            if (pScoreComponent_ != nullptr)
                pScoreComponent_->SetVisible(visible);
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

            /* ボス戦でも Player HP は描画するため、ここでは止めない。 */
            return true;
        }


        void GameplayHudScreen::Draw(RenderContext& rc)
        {
            /* HUD が描画可能かどうかを判定する。*/
            if (!CanDrawHud())
                return;

            /* HUD データソースからロジックを同期する。 */
            SyncFromDataSource();

            /* ボス戦: HP のみ描画（Score/Timer は描画しない）。 */
            if (IsBossStage())
            {
                /* ボス戦のカットインが再生中なら描画しない。 */
                if (IsBossCutInPlaying())
                    return;

                /* HP のみ描画する。 */
                if (pHpComponent_ != nullptr)
                {
                    const Matrix& world = GetRootEntity().GetTransform().GetWorldMatrix();
                    pHpComponent_->OnDraw(rc, world);
                }
                return;
            }

            /* 通常ステージ: チュートリアル後はスコア表示を戻す。 */
            if (auto* pPlayer = FindGO<nsApp::nsActor::nsCharacter::nsPlayer::Player>("player"))
            {
                if (pPlayer->IsTutorialDone())
                    SetScoreVisible(true);
            }

            /* HUD を描画する。*/
            if (pScoreComponent_ != nullptr)
                pScoreComponent_->SetVisible(true);

            /* Entity ツリーを描画する。*/
            GetRootEntity().Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
