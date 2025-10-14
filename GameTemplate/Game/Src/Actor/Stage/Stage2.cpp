#include "stdafx.h"
#include "Src/Actor/Stage/Stage2.h"
#include "Src/Actor/Stage/Stage.h"

namespace
{
	const Vector3 SCALE(1.0f, 1.0f, 1.0f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

Stage2::~Stage2()
{
	DeleteGO(StageCollision_);
}

bool Stage2::Start()
{

	const std::string stagePath = InitStage("Stage2/Stage2");
	StageRender_.Init(stagePath.c_str());
	StagePhysics_.CreateFromModel(StageRender_.GetModel(), StageRender_.GetModel().GetWorldMatrix());
	StageRender_.SetScale(SCALE);


	// コリジョン。
	StageCollision_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	StageCollision_->CreateBox
	(StagePos_ + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE);

	// 座標設定。
	StageRender_.SetPosition(StagePos_);
	initPos_ = StagePos_;

	// コリジョンを破棄。
	StageCollision_->SetIsEnableAutoDelete(false);

	StageRender_.Update();
	return true;
}

void Stage2::Update()
{
	StageRender_.Update();

	// 当たり判定。
	StagePhysics_.SetPosition(StagePos_);

	// コリジョン。
	StageCollision_->SetPosition(StagePos_ + COLLISION_HEIGHT);

}

void Stage2::Render(RenderContext& rc)
{
	StageRender_.Draw(rc);
}