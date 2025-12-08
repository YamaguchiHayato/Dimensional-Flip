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
    const auto ORIGINCAMERA_EVENTTIME = 4.0f; // 演出時間
    const auto TARGET_HEIGHT = 150.0f;
    const auto SWITCH_SPEED = 10.0f;

    const Vector3 SIDE_VIEW_OFFSET = {-1500.0f, 500.0f, 0.0f};
    const Vector3 DEPTH_VIEW_OFFSET = {0.0f, 500.0f, -1500.0f};
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
                        // カメラモードををBOSS戦仕様に。
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
            if (!pBoss_ || !pPlayer_)
                return;

            Vector3 bossPos = pBoss_->GetPos();
            Vector3 playerPos = pPlayer_->GetPlayerPos();

            // 1. bossとPlayerの中間地点を計算し、注視点とする
            Vector3 middlePos = (bossPos + playerPos) * 0.5f;

            // 2. 視点モードに応じたオフセットを取得
            Vector3 targetOffset =
                (currentViewMode_ == BattleViewMode::SidwView) ? SIDE_VIEW_OFFSET : DEPTH_VIEW_OFFSET;

            // 3. 理想位置 = 中間地点 + オフセット
            Vector3 idealPos = middlePos + targetOffset;

            // 4. 滑らかに追従する (Lerp引数の順序に注意し、deltaTimeを適用)
            Vector3 currentPos = pCamera->GetPosition();
            Vector3 newPos = FollowStrategy::Lerp(SWITCH_SPEED * deltaTime, currentPos, idealPos);
            pCamera->SetPosition(newPos);

            // 5. 注視点を中間地点に設定
            Vector3 target = middlePos;
            target.y += TARGET_HEIGHT; // 中間点を見下ろす高さ
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

