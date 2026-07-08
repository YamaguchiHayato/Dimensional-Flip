#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Game.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"
#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"
#include "Src/Production/CutIn/CutInView.h"
#include "StageSetup.h"

namespace
{
    const Vector3 BOSS_PLAYER_LIMIT_MIN(-38.0f, -10.0f, -26.0f);
    const Vector3 BOSS_PLAYER_LIMIT_MAX(38.0f, 500.0f, 26.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MIN(-450.0f, -100.0f, -2000.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MAX(450.0f, 500.0f, 500.0f);

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
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        static bool s_keepPlayerPaused = false;

        bool StageSetup::ShouldKeepPlayerPaused()
        {
            return s_keepPlayerPaused;
        }


        void StageSetup::OnEnter(StageID stageId)
        {
            const auto& master = nsSystem::StageMasterTable::Get(stageId);

            if (master.backgroundType != "Boss")
            {
                isBossStage_ = false;
                isCutInPlaying_ = false;
                s_keepPlayerPaused = false;
                return;
            }

            isBossStage_ = true;
            isCutInPlaying_ = true;
            s_keepPlayerPaused = true;

            SetBossHudVisible(true);

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

            /* ボスステージ終了時の処理。 */
            SetBossHudVisible(true);

            if (Player* player = FindGO<Player>("player"))
                player->ReleaseMoveLimit();

            isBossStage_ = false;
            isCutInPlaying_ = false;
            s_keepPlayerPaused = false;

            SetBossHudVisible(false);
        }


        void StageSetup::Update()
        {
            if (!isBossStage_)
                return;

            /* 旧 BossUIManager::Update() 相当（カットイン中も更新） */
            if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
            {
                if (auto* pHost = pGame->GetBossHudScreenHost())
                    pHost->Update();
            }

            if (!isCutInPlaying_)
                return;

            auto* cutIn = FindGO<CutInView>("CutInView");

            if (cutIn == nullptr || cutIn->IsCutInFinished())
            {
                isCutInPlaying_ = false;
                s_keepPlayerPaused = false;

                ReconnectBossHud();
                SetBossHudVisible(true);

                if (Player* player = FindGO<Player>("player"))
                {
                    const auto& master = nsSystem::StageMasterTable::Get(StageID::sStageEX);
                    player->SetPlayerPos(master.playerStartPosition);
                    player->SetRespwanPos(master.playerStartPosition);
                    player->SetPaused(false);
                }
            }
        }


        void StageSetup::Render(RenderContext& rc)
        {
            if (!isBossStage_)
                return;

            /* 旧 StageEX::Render() — カットイン中も描画（Draw 側でカットイン判定） */
            if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
            {
                if (auto* pHost = FindGO<nsUI::BossHudScreenHost>("BossHudScreenHost"))
                    pHost->Render(rc);
            }
        }
    } // namespace nsStage
} // namespace nsApp
