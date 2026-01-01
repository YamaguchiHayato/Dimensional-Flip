#include "stdafx.h"
#include "Src/Actor/Stage/MainUnit/StageEX.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"
#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"
#include "Src/Direction/CutIn/CutInView.h"

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

                    // カメラをボスモードに変更。
                    pCameraManager_->RequestBossMode(); 
            }

            // プレイヤーの一時停止フラグをセット。
            if (pPlayer_)
                pPlayer_->SetPaused(true);

            // カットインの生成。
            pCutInView_ = NewGO<app::cutIn::CutInView>(0, "CutInView");

            // カットインの寿命(n秒)を設定。
            pCutInView_->SetLifeDuration(4.0f);

            return true;
        }


        void StageEX::Update()
        {

            // 終了判定。
            if (pCutInView_)
            {
                // 時間切れになったら。
                if (pCutInView_->IsCutInFinished())
                {
                    // CutInViewの破棄。
                    // レイヤークラスも一緒に破棄される。
                    // ※レイヤークラス …　CutInViewに所属しているCutInを構成するクラス達。
                    DeleteGO(pCutInView_);
                    pCutInView_ = nullptr;

                    // 一時停止フラグを解除する。
                    if (pPlayer_)
                    {
                        // プレイヤーを所定の位置に移動させる。
                        pPlayer_->SetPlayerPos(nsStageEX::nsPlayer::InitPos);

                        // 一時停止フラグ解除。
                        // Bossのカットイン中は行動を制限させたいため。
                        pPlayer_->SetPaused(false);
                    }
                }
            }

            stageRender_.Update();
        }


        void StageEX::Render(RenderContext& rc)
        {
            stageRender_.Draw(rc);
        }
    }
}

