#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Game.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/Presentation/UI/Screens/BossBriefingScreenHost.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"
#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"
#include "Src/Production/CutIn/CutInView.h"
#include "StageSetup.h"
#include "Src/Core/InputManager.h"

namespace
{
    const Vector3 BOSS_PLAYER_LIMIT_MIN(-38.0f, -10.0f, -26.0f);
    const Vector3 BOSS_PLAYER_LIMIT_MAX(38.0f, 500.0f, 26.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MIN(-450.0f, -100.0f, -2000.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MAX(450.0f, 500.0f, 500.0f);

    bool s_keepPlayerPaused = false;
    bool s_isBriefing = false;

    void SetBossHudVisible(bool visible)
    {
        if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
        {
            if (auto* pHost = pGame->GetBossHudScreenHost())
            {
                if (auto* pScreen = pHost->GetScreen())
                    pScreen->SetVisible(visible);
            }
        }
    }

    void ReconnectBossHud()
    {
        if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
        {
            auto* pData = pGame->GetBossHudData();
            auto* pHost = pGame->GetBossHudScreenHost();
            if (pData == nullptr || pHost == nullptr)
                return;

            auto* pUiScreen = pHost->GetScreen();
            if (pUiScreen == nullptr)
                return;

            auto* pScreen = static_cast<nsApp::nsUI::BossHudScreen*>(pUiScreen);
            pData->SetScreen(pScreen);
            pScreen->Bind(pData);

            if (auto* pBoss = FindGO<app::enemy::Boss>("boss"))
                pData->SetBoss(pBoss);

            pUiScreen->SetVisible(true);
        }
    }

    void DestroyBossBriefingHost()
    {
        if (auto* pHost = FindGO<nsApp::nsUI::BossBriefingScreenHost>("BossBriefingScreenHost"))
            DeleteGO(pHost);
    }
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        bool StageSetup::ShouldKeepPlayerPaused()
        {
            return s_keepPlayerPaused;
        }


        void StageSetup::InitBossBriefingSlides()
        {
            /*
             * 画像は Assets/UI/BossTutorialEvent/ に2枚。
             * 1枚目: ボス戦の流れ
             * 2枚目: 攻撃アイコン説明
             */
            briefingData_.SetSlidePaths({
                "Assets/UI/BossTutorialEvent/flow.DDS",
                "Assets/UI/BossTutorialEvent/BossUI.DDS",
            });
        }


        void StageSetup::StartBriefing()
        {
            introPhase_ = BossIntroPhase::Briefing;
            s_keepPlayerPaused = true;
            s_isBriefing = true;

            SetBossHudVisible(false);

            /* Briefing 中は視点切替を止める。 */
            app::core::InputManager::GetInstance()->SetDimensionFlipFlag(false);

            if (Player* player = FindGO<Player>("player"))
            {
                player->SetPaused(true);
                if (auto* cam = player->GetCameraManager())
                    cam->SetButtonActionControl(false);
            }

            InitBossBriefingSlides();
            briefingController_.Open();
        }


        void StageSetup::StartBattle()
        {
            introPhase_ = BossIntroPhase::Battle;
            s_keepPlayerPaused = false;
            s_isBriefing = false;

            /* ボス戦開始時に視点切替を戻す。 */
            app::core::InputManager::GetInstance()->SetDimensionFlipFlag(true);

            ReconnectBossHud();
            SetBossHudVisible(true);

            if (Player* player = FindGO<Player>("player"))
            {
                const auto& master = nsSystem::StageMasterTable::Get(StageID::sStageEX);
                player->SetPlayerPos(master.playerStartPosition);
                player->SetRespwanPos(master.playerStartPosition);
                player->SetPaused(false);

                if (auto* cam = player->GetCameraManager())
                    cam->SetButtonActionControl(true);
            }
        }


        void StageSetup::OnEnter(StageID stageId)
        {
            const auto& master = nsSystem::StageMasterTable::Get(stageId);

            if (master.backgroundType != "Boss")
            {
                isBossStage_ = false;
                introPhase_ = BossIntroPhase::CutIn;
                s_keepPlayerPaused = false;
                return;
            }

            isBossStage_ = true;
            introPhase_ = BossIntroPhase::CutIn;
            s_keepPlayerPaused = true;

            briefingController_.Initialize(&briefingData_);
            InitBossBriefingSlides();

            SetBossHudVisible(false);

            Player* player = FindGO<Player>("player");
            if (player == nullptr)
                return;

            CameraManager* cam = player->GetCameraManager();
            if (cam != nullptr)
            {
                cam->Request2DMode();
                cam->SetCameraRange(BOSS_CAMERA_LIMIT_MIN, BOSS_CAMERA_LIMIT_MAX);
            }

            player->SetMoveLimit(BOSS_PLAYER_LIMIT_MIN, BOSS_PLAYER_LIMIT_MAX);
            player->SetPaused(true);
        }


        void StageSetup::OnLeave(StageID stageId)
        {
            (void) stageId;

            if (!isBossStage_)
                return;

            briefingController_.Close();
            DestroyBossBriefingHost();

            if (Player* player = FindGO<Player>("player"))
                player->ReleaseMoveLimit();

            isBossStage_ = false;
            introPhase_ = BossIntroPhase::CutIn;
            s_keepPlayerPaused = false;

            SetBossHudVisible(false);
        }


        void StageSetup::Update()
        {
            if (!isBossStage_)
                return;

            /* Briefing 中 */
            if (introPhase_ == BossIntroPhase::Briefing)
            {
                briefingController_.BeginInputFrame();
                briefingController_.Update();

                if (auto* pHost = FindGO<nsUI::BossBriefingScreenHost>("BossBriefingScreenHost"))
                    pHost->Update();

                if (briefingController_.IsFinished())
                    StartBattle();

                return;
            }

            /* 本番ボス戦中の Boss HUD 更新 */
            if (introPhase_ == BossIntroPhase::Battle)
            {
                if (auto* pGame = FindGO<nsCore::Game>("game"))
                {
                    if (auto* pHost = pGame->GetBossHudScreenHost())
                        pHost->Update();
                }
            }

            /* カットイン終了 → Briefing 開始 */
            if (introPhase_ != BossIntroPhase::CutIn)
                return;

            auto* cutIn = FindGO<CutInView>("CutInView");
            if (cutIn == nullptr || cutIn->IsCutInFinished())
                StartBriefing();
        }

        void StageSetup::Render(RenderContext& rc)
        {
            if (!isBossStage_)
                return;

            /* Briefing スライド UI */
            if (introPhase_ == BossIntroPhase::Briefing)
            {
                if (auto* pHost = FindGO<nsUI::BossBriefingScreenHost>("BossBriefingScreenHost"))
                    pHost->Render(rc);
                return;
            }

            /* 本番ボス HUD */
            if (introPhase_ == BossIntroPhase::Battle)
            {
                if (auto* pGame = FindGO<nsCore::Game>("game"))
                {
                    if (auto* pHost = FindGO<nsUI::BossHudScreenHost>("BossHudScreenHost"))
                        pHost->Render(rc);
                }
            }
        }
    } // namespace nsStage
} // namespace nsApp
