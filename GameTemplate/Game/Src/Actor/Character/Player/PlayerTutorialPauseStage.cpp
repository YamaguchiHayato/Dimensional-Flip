#include "stdafx.h"

#include "PlayerTutorialPauseStage.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Wall.h"
#include "Src/Core/SoundManager.h"

// 必要なヘッダの読み込み
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/InputManager.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"
#include "Src/UI/Tutorial/TutorialSequencer.h"
#include "Src/UI/Tutorial/TutorialCompleteUI.h"

namespace app
{
    namespace state
    {
        static bool isMoveTutorialDone_ = false;


        void PlayerTutorialPauseStage::Enter()
        {
            app::core::InputManager::GetInstance()->SetDimensionFlipFlag(true);
            if (pPlayer_ && pPlayer_->GetCameraManager())
                pPlayer_->GetCameraManager()->SetButtonActionControl(true);

            // シーケンサー生成
            pTutorialSequencer_ = NewGO<app::nsUI::TutorialSequencer>(0, "TutorialSequencer");

            // UIのポインタも取得しておく
            pTutorialMessageUI_ = FindGO<app::nsUI::TutorialMessageUI>("TutorialMessageUI");

            // Complete演出UIをセット。
            pTutorialCompleteUI_ = NewGO<app::nsUI::TutorialCompleteUI>(0, "TutorialCompleteUI");
            if (pTutorialCompleteUI_ == nullptr)
            {
                pTutorialCompleteUI_ = NewGO<app::nsUI::TutorialCompleteUI>(0, "TutorialCompleteUI");
            }

            // チュートリアルテキストの切り替え設定
            TutorialTextChange();
        }

        
        void PlayerTutorialPauseStage::Update()
        {
            Vector3 stickL = {g_pad[0]->GetLStickXF(), 0.0f, g_pad[0]->GetLStickYF()};
            pPlayer_->SetKeyDirection(stickL);

            float speedRate = 1.0f; // 地上の倍率

            if (!pPlayer_->GetCharacterController().IsOnGround())
                speedRate = 0.7f;

            pPlayer_->CalculateMovement(stickL);

            // ジャンプ処理の適応。
            JumpUpdatingProcessAtTutorial();

            // 移動処理の適応。
            pPlayer_->ApplyMovement();

            // アニメーションの更新処理。
            AnimationUpdatingProcess();

            // 視点切り替え
            ViewpointChangeUpdatingProcessAtTutorial();

            if (pPlayer_ && pPlayer_->GetCameraManager())
                pPlayer_->GetCameraManager()->Update();
        }


        void PlayerTutorialPauseStage::Exit()
        {
            // シーケンサー削除
            if (pTutorialSequencer_)
            {
                DeleteGO(pTutorialSequencer_);
                pTutorialSequencer_ = nullptr;
            }
        }


        bool PlayerTutorialPauseStage::RequestID(uint8_t& request)
        {
            if (pTutorialSequencer_ && pTutorialSequencer_->IsAllFinished())
            {
                request = EnPlayerState::enState_Idle;
                return true;
            }

            return false;
        }


        // @TODO: リファクタリング検討　↓4
        void PlayerTutorialPauseStage::AnimationUpdatingProcess()
        {
            // 空中時の処理。
            if (!pPlayer_->GetCharacterController().IsOnGround())
            {
                const Vector3& stick = pPlayer_->GetKeyDirection();

                if (stick.x < -0.01f)
                    pPlayer_->SetCurrentIndex(1);
                else if (stick.x > 0.01f)
                    pPlayer_->SetCurrentIndex(5);

                return;
            }

            // タイマーを進める。
            currentTime_ += g_gameTime->GetFrameDeltaTime();
            if (currentTime_ >= 0.4f)
                currentTime_ = 0.0f;

            // 直前にセットした入力方向を取得する。
            const Vector3& stick = pPlayer_->GetKeyDirection();

            // 移動入力がある場合。
            if (fabsf(stick.x) > 0.1f || fabsf(stick.z) > 0.1f)
            {
                // 左方向。
                if (stick.x < 0.0f)
                {
                    if (currentTime_ < 0.1f)
                        pPlayer_->SetCurrentIndex(2);

                    else if (currentTime_ < 0.2f)
                        pPlayer_->SetCurrentIndex(0);

                    else if (currentTime_ < 0.3f)
                        pPlayer_->SetCurrentIndex(3);

                    else
                        pPlayer_->SetCurrentIndex(0);
                }

                // 右方向。
                else
                {
                    if (currentTime_ < 0.1f)
                        pPlayer_->SetCurrentIndex(6);

                    else if (currentTime_ < 0.2f)
                        pPlayer_->SetCurrentIndex(4);

                    else if (currentTime_ < 0.3f)
                        pPlayer_->SetCurrentIndex(7);

                    else
                        pPlayer_->SetCurrentIndex(4);
                }
            }

            // 停止中。
            else
            {
                // 向きに合わせて待機モーションに。
                if (stick.x < 0.0f)
                    pPlayer_->SetCurrentIndex(0);

                else
                    pPlayer_->SetCurrentIndex(4);
            }
        }


        void PlayerTutorialPauseStage::JumpUpdatingProcessAtTutorial()
        {
            Vector3& speed = pPlayer_->GetMoveSpeed();
            bool isGround = pPlayer_->GetCharacterController().IsOnGround();

            if (isGround)
            {
                if (speed.y < 0.0f)
                    speed.y = 0.0f;

                // ジャンプ開始 (JumpStateと同じ 150.0f を設定)
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    speed.y = pPlayer_->GetJumpPower();

                    app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Player_Jump);

                    pPlayer_->SetCurrentIndex(pPlayer_->GetKeyDirection().x < 0 ? 0 : 4);
                }
            }
            else
            {
                if (speed.y > 0.0f) // 上昇中
                {
                    // Aボタンを離しているなら重力を強くする
                    if (!g_pad[0]->IsPress(enButtonA))
                        speed.y -= 8.0f * 2.0f; // GLAVITY * CUT
                    else
                        speed.y -= 8.0f; // 通常重力
                }
                else // 下降中
                {
                    speed.y -= 8.0f * 1.2f; // GLAVITY * Jump::GLAVITY
                }

                // 落下速度制限
                if (speed.y < -200.0f)
                    speed.y = -200.0f; // FALLINGSPEED
            }
        }


        void PlayerTutorialPauseStage::ViewpointChangeUpdatingProcessAtTutorial()
        {
            auto* pWall = FindGO<app::stage::Wall>("wall");

            if (pPlayer_->TryFlipDimension(true))
            {
                if (pWall)
                {
                    pWall->SwitchMode();
                }
            }
        }


        void PlayerTutorialPauseStage::TutorialTextChange()
        {
            if (pTutorialSequencer_)
            {
                // 移動処理。
                if (!isMoveTutorialDone_ && pPlayer_->GetPlayerPos().x < 65.0f)
                {
                    pTutorialSequencer_->AddStep(L"Stick: Move",
                    [this]() -> bool
                    {
                        // 現在のX座標が 65.0f を超えたら完了 (Complete演出へ)
                        // これにより、坂の手前でちょうど「Stick: Move」が消えます
                        if (pPlayer_->GetPlayerPos().x > 65.0f)
                        {
                            isMoveTutorialDone_ = true; // 二度と出ないようにフラグを立てる
                            return true;
                        }
                        return false;
                    });
                }


                // 視点切替。
                pTutorialSequencer_->AddStep(L"Flip View",
                [this]() -> bool
                {
                    // 1. 壁の手前までは文字を隠す
                    if (pPlayer_->GetPlayerPos().x < 160.0f)
                    {
                        if (pTutorialMessageUI_)
                            pTutorialMessageUI_->SetMessage(L"");
                        return false;
                    }

                    // 2. 文字を表示
                    if (pTutorialMessageUI_)
                        pTutorialMessageUI_->SetMessage(L"Flip View");

                    if (pPlayer_->GetCameraManager())
                    {
                        auto mode = pPlayer_->GetCameraManager()->GetCurrentCameraMode();
                        // 3Dモード(mode3D)になっていれば完了
                        return (mode == CameraMode::mode3D);
                    }

                    return false;
                });


                // フリーアクション。
                pTutorialSequencer_->AddStep(L"Free Action", [this]()->bool
                {
                    // ゴールのスターの獲得を完了条件とする。
                    if (pPlayer_->GetPlayerPos().x > 400.0f)
                        return true;

                    return false;
                });
            }
        }
    } // namespace state
} // namespace app
