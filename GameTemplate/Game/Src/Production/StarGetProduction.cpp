#include "stdafx.h"
#include "StarGetProduction.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Star.h"

#include "Src/Production/Fade.h"
#include "Src/Production/StageClear.h"
#include "Src/Production/GameClear.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"

#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"


namespace
{
    const auto PLAYER_ROTATION_SPEED = 1.2f; // プレイヤー回転速度（度/秒）
}

namespace app
{
    namespace production
    {
        void StarGetProduction::StartSequence(Player* pPlayer, Star* pStar)
        {
            pPlayer_ = pPlayer;
            pStar_ = pStar;
            starPos_ = pStar_->GetStarPosition();
            playerStartPos_ = pPlayer_->GetPlayerPos();
            initialCameraHeight_ = g_camera3D->GetHeight();

            pPlayer_->SetPaused(true);

            // 1. 演出開始時にスターの座標で背景を固定する
            auto* pBG = FindGO<nsApp::nsStage::nsScrollBackGround::ScrollStageBackGround>("Normal");
            if (pBG)
                pBG->SetOverrideTrackingPosition(starPos_);

            // 2. カメラのプレイヤー追従を停止させる
            auto* pCamMgr = FindGO<CameraManager>("cameramanager");
            if (pCamMgr)
                pCamMgr->SetTracking(false);


            currentPhase_ = StarGetPhase::Approach;
            timer_ = 0.0f;
        }


        void StarGetProduction::Update()
        {
            // 経過時間の取得。
            auto deltaTime = g_gameTime->GetFrameDeltaTime();

            switch (currentPhase_)
            {
            case StarGetPhase::Approach:
                UpdateApproach(deltaTime);
                break;

            case StarGetPhase::Orbit:
                UpdateOrbit(deltaTime);
                break;

            case StarGetPhase::Finish:
            case StarGetPhase::FadeOut:
                UpdateFinish(deltaTime);
                break;

            default:
                break;
            }

            // Playerの2Dレンダー更新。
            if (pPlayer_ && pPlayer_->GetCharacter2DRender())
                pPlayer_->GetCharacter2DRender()->Update();
        }


        void StarGetProduction::UpdateApproach(float deltaTime)
        {
            timer_ += deltaTime;
            const float duration = PLAYER_ROTATION_SPEED;
            float rate = min(timer_ / duration, 1.0f);

            // カメラズーム
            float targetHeight = 20.0f;
            float currentHeight = initialCameraHeight_ + (targetHeight - initialCameraHeight_) * rate;
            g_camera3D->SetHeight(currentHeight);
            g_camera3D->SetWidth(currentHeight * (16.0f / 9.0f));

            g_camera3D->SetTarget(starPos_); // 注視点は常にスター

            float radius = 20.0f;
            Vector3 orbitStartPos = starPos_ + Vector3(radius, 0.0f, 0.0f);
            orbitStartPos.y = playerStartPos_.y;

            Vector3 currentPos = playerStartPos_ + (orbitStartPos - playerStartPos_) * rate;
            pPlayer_->SetPlayerPos(currentPos);

            // カメラ注視点の補正（プレイヤーとスターの中間地点を注視）
            Vector3 midPoint = (starPos_ + currentPos) * 0.5f;
            Vector3 currentLookAt = playerStartPos_ + (midPoint - playerStartPos_) * rate;
            g_camera3D->SetTarget(currentLookAt);

            if (rate >= 1.0f)
            {
                currentPhase_ = StarGetPhase::Orbit;
                timer_ = 0.0f;
            }
        }


        void StarGetProduction::UpdateFinish(float deltaTime)
        {
            // 経過時間の更新
            timer_ += deltaTime;

            // 常にスターを注視し、プレイヤーを隣に固定し続ける（位置戻り防止）
            g_camera3D->SetTarget(starPos_);
            Vector3 finalPos = starPos_ + Vector3(-5.0f, 0.0f, 0.0f); // 少し左に
            finalPos.y = playerStartPos_.y;
            pPlayer_->SetPlayerPos(finalPos);

            if (pStar_)
                pStar_->UpdateMovement(deltaTime);

            // --- 演出終了とフェード ---
            // 演出が終わっても、カメラや背景の固定を「解除しない」ことで、
            // 暗転するまで今の見た目をキープします
            if (timer_ >= 1.0f && currentPhase_ == StarGetPhase::Finish)
            {
                auto* pBG = FindGO<nsApp::nsStage::nsScrollBackGround::ScrollStageBackGround>("Normal");
                if (pBG)
                    pBG->ClearOverride();

                NewGO<StageClear>(0, "stage_clear_logo");


                Fade* pFade = SceneManager::GetInstance()->GetFade();
                if (pFade && pFade->GetFadeState() != FadeState::Fade_Out)
                {
                    pFade->StartFadeOut();
                }
                currentPhase_ = StarGetPhase::FadeOut;
            }


            // フェードアウトが十分に進んだら、データ収集とシーン遷移
            if (timer_ >= 2.5f)
            {
                // データを集めて送信
                CollectAndSendResultData();

                // 座標セットなど
                Vector3 finalPos = starPos_ + Vector3(-5.0f, 0.0f, 0.0f);
                finalPos.y = playerStartPos_.y;
                pPlayer_->SetPlayerPos(finalPos);

                auto* pCamMgr = FindGO<CameraManager>("cameramanager");
                if (pCamMgr)
                    pCamMgr->SetTracking(true);

                SceneManager::GetInstance()->ChangeScene(SceneID::sResult);
                DeleteGO(this);
            }
        }


        void StarGetProduction::CollectAndSendResultData()
        {
            // 1. UIインスタンスを取得
            auto* pNumberUI = NumberUI::GetInstance();
            auto* pScoreUI = ScoreUI::GetInstance();

            // 2. データ格納用
            nsApp::nsStage::StageResultData finalData;

            // 3. タイム収集
            if (pNumberUI)
                finalData.clearTime_ = pNumberUI->GetTimer();

            // 4. スコア収集
            if (pScoreUI)
                finalData.baseScore_ = (int) pScoreUI->GetScore();

            // 5. StageManagerに送信
            auto* pStageManager = nsApp::nsStage::StageManager::GetInstance();
            if (pStageManager)
                pStageManager->SetStageResult(finalData);
        }


        void StarGetProduction::UpdateOrbit(float deltaTime)
        {
            timer_ += deltaTime;
            const float duration = PLAYER_ROTATION_SPEED;
            float rate = min(timer_ / duration, 1.0f);
            g_camera3D->SetTarget(starPos_);

            float angle = rate * 3.14159f; // 180度回転
            const float radius = 15.0f;    // 半径を15.0fに固定

            Vector3 orbitPos;
            orbitPos.x = starPos_.x + cosf(angle) * radius; // 終点はスターの左側 (-15.0f)
            orbitPos.z = starPos_.z - sinf(angle) * radius; // 手前を回る
            orbitPos.y = playerStartPos_.y;
            pPlayer_->SetPlayerPos(orbitPos);

            Vector3 midPoint = (starPos_ + orbitPos) * 0.5f;
            g_camera3D->SetTarget(midPoint);

            if (rate >= 1.0f)
            {
                currentPhase_ = StarGetPhase::Finish;
                timer_ = 0.0f;
                pPlayer_->SetCurrentIndex(0);
            }
        }


    } // namespace production
} // namespace app
