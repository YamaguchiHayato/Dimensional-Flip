#include "stdafx.h"

#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Game.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/Production/CutIn/CutInView.h"
#include "StageSetup.h"

namespace
{
    const Vector3 BOSS_PLAYER_LIMIT_MIN(-38.0f, -10.0f, -26.0f);
    const Vector3 BOSS_PLAYER_LIMIT_MAX(38.0f, 500.0f, 26.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MIN(-450.0f, -100.0f, -2000.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MAX(450.0f, 500.0f, 500.0f);
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

            /* ボス HUD を表示する */
            if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
            {
                if (auto* pHost = pGame->GetBossHudScreenHost())
                {
                    if (auto* pScreen = pHost->GetBossHudScreen())
                        pScreen->SetVisible(true);
                }
            }

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

            if (Player* player = FindGO<Player>("player"))
                player->ReleaseMoveLimit();

            isBossStage_ = false;
            isCutInPlaying_ = false;
            s_keepPlayerPaused = false;

            /* ボス HUD を非表示にする */
            if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
            {
                if (auto* pHost = pGame->GetBossHudScreenHost())
                {
                    if (auto* pScreen = pHost->GetBossHudScreen())
                        pScreen->SetVisible(false);
                }
            }
        }

        void StageSetup::Update()
        {
            if (!isBossStage_)
                return;

            if (!isCutInPlaying_)
                return;

            auto* cutIn = FindGO<CutInView>("CutInView");

            if (cutIn == nullptr || cutIn->IsCutInFinished())
            {
                isCutInPlaying_ = false;
                s_keepPlayerPaused = false;

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
            (void) rc;
            /* Boss HUD は BossHudScreenHost の GO Render で描画 */
        }
    } // namespace nsStage
} // namespace nsApp
