#include "stdafx.h"

// キャラクタークラス。
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"


// 管理クラス。
#include "Src/Core/SceneManager.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/StageManager.h"


// カメラクラス。
#include "Src/Camera/BossCameraStrategy.h"
#include "Src/Camera/FollowStrategy.h" 

namespace
{
    static constexpr auto ORIGINCAMERA_EVENTTIME = 4.0f; // 演出時間
    static constexpr auto TARGET_HEIGHT = 150.0f;
    static constexpr auto SWITCH_SPEED = 10.0f;

    const Vector3 SIDE_VIEW_OFFSET = {-500.0f, 150.0f, 0.0f};  // 側面視点オフセット
    const Vector3 DEPTH_VIEW_OFFSET = {0.0f, 150.0f, -500.0f}; // 奥行き視点オフセット
}; 

namespace app
{
    namespace camera
    {
        BossCameraStrategy::BossCameraStrategy(Player* pPlayer) : pPlayer_(pPlayer) {}

        bool BossCameraStrategy::Start()
        {
            // ボスを検索
            pBoss_ = FindGO<Boss>("boss");
            if (!pBoss_)
                K2_LOG("BossCameraStrategy::Start() Boss not found.\n");

            // プレイヤーの確認
            if (!pPlayer_)
                K2_LOG("BossCameraStrategy::Start() Player is null.\n");

            // 状態リセット
            currenState_ = BossCameraState::Appearance;
            eventTimerLapse_ = 0.0f;
            currentViewMode_ = BattleViewMode::SidwView;

            return true;
        }


        void BossCameraStrategy::Update(nsK2EngineLow::Camera* pCamera, const float deltaTime)
        {
            if (StageManager::GetInstance()->GetCurrentStageID() != StageID::sStageEX)
                return;

            if (!pBoss_)
                return;

            // 現在の状態に応じて処理を分岐
            BossCameraGetState(pCamera, deltaTime);
        }


        void BossCameraStrategy::BossAppearanceCamera(nsK2EngineLow::Camera* pCamera)
        {
            if (!pBoss_ || !pCamera)
                return;

            // 時間を進める
            eventTimerLapse_ += g_gameTime->GetFrameDeltaTime();

            // --- 演出用カメラワーク ---
            // ボスの斜め上からの視点
            Vector3 highAngleOffset(0.0f, 500.0f, -300.0f);
            Vector3 targetPos = pBoss_->GetPos() + highAngleOffset;

            // 現在位置から滑らかに移動
            Vector3 currentPos = pCamera->GetPosition();
            float t = 2.0f * g_gameTime->GetFrameDeltaTime();
            Vector3 newPos = FollowStrategy::Lerp(t, currentPos, targetPos);
            pCamera->SetPosition(newPos);

            // ボスを見下ろす
            Vector3 lookAt = pBoss_->GetPos();
            lookAt.y += 50.0f;
            pCamera->SetTarget(lookAt);

            // --- 終了判定と切り替え ---
            if (eventTimerLapse_ >= ORIGINCAMERA_EVENTTIME)
            {
                // 演出終了。戦闘用（俯瞰）カメラへ切り替えリクエスト。
                if (pPlayer_)
                {
                    CameraManager* pCamMgr = pPlayer_->GetCameraManager();
                    if (pCamMgr)
                    {
                        // ★ここでStageExMode(俯瞰視点)へ移行
                        pCamMgr->RequestStageExMode();
                    }
                }

                // ステート更新（切り替えが即座に行われるため、実質ここは通りませんが念のため）
                eventTimerLapse_ = 0.0f;
                currenState_ = BossCameraState::Battle;
            }
        }


        void BossCameraStrategy::BattleCamera(nsK2EngineLow::Camera* pCamera, const float deltaTime)
        {
            // 基本的には RequestStageExMode で切り替わるため、ここは予備のロジックになります
            if (!pBoss_)
                return;

            Vector3 bossPos = pBoss_->GetPos();
            Vector3 targetOffset =
                (currentViewMode_ == BattleViewMode::SidwView) ? SIDE_VIEW_OFFSET : DEPTH_VIEW_OFFSET;
            Vector3 idealPos = bossPos + targetOffset;

            Vector3 currentPos = pCamera->GetPosition();
            Vector3 newPos = FollowStrategy::Lerp(SWITCH_SPEED * deltaTime, idealPos, currentPos);
            pCamera->SetPosition(newPos);

            Vector3 target = bossPos;
            target.y += TARGET_HEIGHT;
            pCamera->SetTarget(target);
        }


        void BossCameraStrategy::BossCameraGetState(nsK2EngineLow::Camera* pCamera, const float deltaTime)
        {
            switch (currenState_)
            {
            case BossCameraState::Appearance:
                BossAppearanceCamera(pCamera);
                break;

            case BossCameraState::Battle:
                BattleCamera(pCamera, deltaTime);
                break;
            }
        }

    }
}

