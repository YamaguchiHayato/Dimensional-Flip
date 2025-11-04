#include "stdafx.h"
#include "Src/Actor/Stage/Stage2.h"
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"

namespace
{
    const Vector3 STARTPOS(0.0f, 0.0f, 0.0f);
	const Vector3 SCALE(0.0125f, 0.0125f, 0.0125f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

Stage2::~Stage2()
{
    DeleteGO(pStageCollision_);
}

bool Stage2::Start()
{

	const std::string stagePath = InitStage("Stage2/stage2");
	stageRender_.Init(stagePath.c_str());

    // 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;

    // 大きさ設定。
    stageRender_.SetScale(SCALE);

    stageRender_.Update();

	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());



	// コリジョン。
	pStageCollision_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	pStageCollision_->CreateBox
	(   stagePos_ + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE );


	// コリジョンを破棄。
	pStageCollision_->SetIsEnableAutoDelete(false);

    pPlayer_ = FindGO<Player>("player");

    // ステージ開始時のPlayerとCameraの移動処理。
    if (pPlayer_)
    {
        // CCごと開始位置に移動させる。
        pPlayer_->GetPlayerCC().SetPosition(STARTPOS);

        // プレイヤーのカメラマネージャーを更新。
        // デフォルトの2Dモードに戻す処理を呼ぶ。
        auto* pCamManager = pPlayer_->GetCameraManager(); 
        if (pCamManager)
        {
            // 2Dモードに戻す。
            pCamManager->Request2DMode();
            // カメラの角度をリセット。
            pCamManager->Request2DRotation(0.0f);
        }
    }

	stageRender_.Update();
	return true;
}

void Stage2::Update()
{
    // モデルの大きさを設定する。
    stageRender_.SetScale(SCALE);
    // モデルの座標を設定する。
    stageRender_.SetPosition(stagePos_);
	// 当たり判定。
	stagePhysics_.SetPosition(stagePos_);
	// コリジョン。
	pStageCollision_->SetPosition(stagePos_ + COLLISION_HEIGHT);
    // モデルを更新する。
    stageRender_.Update();
}

void Stage2::Render(RenderContext& rc)
{
	stageRender_.Draw(rc);
}
