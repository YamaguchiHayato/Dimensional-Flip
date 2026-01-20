#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"
#include "Src/Actor/Stage/MainUnit/StageEX.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Production/CutIn/CutInView.h"


// カメラ。
#include "Src/Core/CameraManager.h"
#include "Src/Camera/SideCameraStrategy.h"

namespace
{
    const Vector3 SCALE = Vector3::One;

    const Vector3 PLAYER_LIMIT_MIN = {-38.0f, -10.0f, -26.0f};
    const Vector3 PLAYER_LIMIT_MAX = {38.0f, 500.0f, 26.0f};

     const Vector3 CAMERA_LIMIT_MIN = {-450.0f, -100.0f, -2000.0f};
     const Vector3 CAMERA_LIMIT_MAX = {450.0f, 500.0f, 500.0f};
} // namespace


namespace app
{
    namespace stage
    {
        bool StageEX::Start()
        {
            // モデル読み込み
            const std::string stagePath = InitStage("StageEX/stageEX");
            stageRender_.Init(stagePath.c_str());

            // 座標設定
            stageRender_.SetPosition(stagePos_);
            initPos_ = stagePos_;

            rot_.SetRotationDegY(-90.0f);
            stageRender_.SetRotation(rot_);

            // 大きさ設定
            stageRender_.SetScale(SCALE);
            stageRender_.Update();
            stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());

            // プレイヤー検索
            pPlayer_ = FindGO<Player>("player");

            // ボスの生成
            CreateBossInstance();

            // カメラマネージャー取得
            if (pPlayer_)
            {
                pCameraManager_ = pPlayer_->GetCameraManager();
                if (pCameraManager_)
                {
                    // 2Dモードに設定
                    pCameraManager_->Request2DMode();

                    // スクリーンロック設定
                    pCameraManager_->SetCameraRange(CAMERA_LIMIT_MIN, CAMERA_LIMIT_MAX);
                }

                // Playerの初期位置を設定。
                pPlayer_->SetPlayerPos(nsStageEX::nsPlayer::InitPos);
                // 移動制限。
                pPlayer_->SetMoveLimit(PLAYER_LIMIT_MIN, PLAYER_LIMIT_MAX);


                if (pPlayer_)
                    pPlayer_->SetPaused(true);
            }

            // トリガー生成
            auto trigger = NewGO<DimensionTrigger>(0, "trigger");
            trigger->SetTriggerPos(Vector3::Zero);

            // カットイン生成
            pCutInView_ = NewGO<app::cutIn::CutInView>(0, "CutInView");
            pCutInView_->SetLifeDuration(4.0f);

            return true;
        }


        void StageEX::Update()
        {
            if (pCutInView_)
            {
                if (pCutInView_->IsCutInFinished())
                {
                    DeleteGO(pCutInView_);
                    pCutInView_ = nullptr;

                    if (pPlayer_)
                    {
                        pPlayer_->SetPlayerPos(nsStageEX::nsPlayer::InitPos);
                        pPlayer_->SetPaused(false);
                    }
                }
            }

            // ステージ更新
            // Startで設定済みですが、念のため毎フレーム更新
            rot_.SetRotationDegY(-90.0f);
            stageRender_.SetRotation(rot_);
            // スケール設定。
            stageRender_.SetScale(SCALE);
            // 更新。
            stageRender_.Update();
        }


        void StageEX::Render(RenderContext& rc)
        {
            stageRender_.Draw(rc);
        }
    }
}

