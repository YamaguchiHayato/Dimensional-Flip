#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/BossUIManager.h"
#include "Src/Core/CameraManager.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/Production/CutIn/CutInView.h"
#include "StageSetup.h"

namespace
{
    // 旧 StageEX.cpp と同じ値
    const Vector3 BOSS_PLAYER_LIMIT_MIN(-38.0f, -10.0f, -26.0f);
    const Vector3 BOSS_PLAYER_LIMIT_MAX(38.0f, 500.0f, 26.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MIN(-450.0f, -100.0f, -2000.0f);
    const Vector3 BOSS_CAMERA_LIMIT_MAX(450.0f, 500.0f, 500.0f);
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        // 静的フラグ（Game.cpp から参照するため）
        static bool s_keepPlayerPaused = false;


        bool StageSetup::ShouldKeepPlayerPaused()
        {
            return s_keepPlayerPaused;
        }


        void StageSetup::OnEnter(StageID stageId)
        {
            const auto& master = nsSystem::StageMasterTable::Get(stageId);

            // BackgroundType が Boss でなければ何もしない
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

            Player* player = FindGO<Player>("player");
            if (player == nullptr)
                return;

            // --- 旧 StageEX::Start() のカメラ設定 ---
            CameraManager* cam = player->GetCameraManager();
            if (cam != nullptr)
            {
                cam->Request2DMode(); // 2D サイドカメラ（Boss 検出で BossCamera に切替）
                cam->SetCameraRange(BOSS_CAMERA_LIMIT_MIN, BOSS_CAMERA_LIMIT_MAX);
            }

            // --- プレイヤー移動制限 ---
            player->SetMoveLimit(BOSS_PLAYER_LIMIT_MIN, BOSS_PLAYER_LIMIT_MAX);

            // --- カットイン中は動かさない ---
            player->SetPaused(true);
        }


        void StageSetup::OnLeave(StageID stageId)
        {
            (void) stageId;

            if (!isBossStage_)
                return;

            // ボスステージを出るときは制限を戻す
            if (Player* player = FindGO<Player>("player"))
                player->ReleaseMoveLimit();

            isBossStage_ = false;
            isCutInPlaying_ = false;
            s_keepPlayerPaused = false;
        }


        void StageSetup::Update()
        {
            if (!isBossStage_)
                return;

            // 旧 StageEX::Update() にあった Boss UI 更新
            app::nsUI::BossUIManager::GetInstance().Update();

            // カットイン終了待ち
            if (!isCutInPlaying_)
                return;

            auto* cutIn = FindGO<app::cutIn::CutInView>("CutInView");

            // CutInView は終了時に自分で DeleteGO するので nullptr でも終了扱い
            if (cutIn == nullptr || cutIn->IsCutInFinished())
            {
                isCutInPlaying_ = false;
                s_keepPlayerPaused = false;

                if (Player* player = FindGO<Player>("player"))
                {
                    // 旧 StageEX: カットイン後に開始位置へ
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

            // 旧 StageEX::Render() にあった Boss UI 描画
            app::nsUI::BossUIManager::GetInstance().Draw(rc);
        }
    } // namespace nsStage
} // namespace nsApp
