#include "stdafx.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Character/Player.h"
namespace
{
	const Vector3 SCALE(1.0f, 1.0f, 1.0f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

Stage1::~Stage1()
{
    DeleteGO(pStageCollision_);
}

bool Stage1::Start()
{
	const std::string stagePath = InitStage("Stage1/Stage1");
	stageRender_.Init(stagePath.c_str());

	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());
	stageRender_.SetScale(SCALE);

	// コリジョン。
	pStageCollision_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	pStageCollision_->CreateBox
	(   stagePos_ + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE );

	// 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;

	// コリジョンを破棄。
	pStageCollision_->SetIsEnableAutoDelete(false);

	stageRender_.Update();
	return true;
}

void Stage1::Update()
{

	stageRender_.Update();

	// 当たり判定。
	stagePhysics_.SetPosition(stagePos_);

	// コリジョン。
	pStageCollision_->SetPosition(stagePos_ + COLLISION_HEIGHT);

}

void Stage1::Render(RenderContext& rc)
{
	stageRender_.Draw(rc);
}
