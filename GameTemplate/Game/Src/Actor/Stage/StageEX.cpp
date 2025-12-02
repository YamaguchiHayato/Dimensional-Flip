#include "stdafx.h"
#include "Src/Actor/Stage/StageEX.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"
namespace
{
    const Vector3 SCALE(Vector3(10.0f, 10.0f, 10.0f));
}

namespace app
{
    namespace stage
    {
        bool StageEX::Start()
        {
            // ステージ2モデル。
            const std::string stagePath = InitStage("StageEX/stageEX");
            stageRender_.Init(stagePath.c_str());

            // 座標設定。
            stageRender_.SetPosition(stagePos_);
            initPos_ = stagePos_;

            // 大きさ設定。
            stageRender_.SetScale(SCALE);
            stageRender_.Update();
            stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());

            // カメラの設定の修正。
            pPlayer_ = FindGO<Player>("player");

            // ボスの生成。
            BossInstance();

            if (pPlayer_)
            {
                // プレイヤーのカメラマネージャーを取得。  
                pCameraManager_ = pPlayer_->GetCameraManager();
                if (pCameraManager_)
                    pCameraManager_->RequestBossMode(); 
            }

            return true;
        }

        void StageEX::Update()
        {
            stageRender_.Update();
        }

        void StageEX::Render(RenderContext& rc)
        {
            stageRender_.Draw(rc);
        }
    }
}

