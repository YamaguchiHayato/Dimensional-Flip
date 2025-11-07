#include "stdafx.h"
#include "Src/Actor/Stage/Stage2.h"
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"
#include "Game.h"
// ギミック。
#include "Src/Actor/Stage/Gimmick/RotationFool.h"

namespace
{
    const Vector3 STARTPOS(0.0f, 101.0f, 0.0f);
    //const Vector3 SCALE(1.0f, 1.0f, 1.0f);
    const Vector3 SCALE(0.125f, 0.125f, 0.125f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

namespace Stage2GimmickPos
{
    // 回転トリック。
    namespace RotationFoolPosition
    {
        const Vector3 Pos1(300.0f, 0.0f, -0.0f);
    }
} 

Stage2::~Stage2()
{
    DeleteGO(pStageCollision_);
}

bool Stage2::Start()
{
    // ステージ2に使うオブジェクトの生成。
    // ステージ2モデル。
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


    RotationFoolNewGO();
	return true;
}

void Stage2::Update()
{
    // モデルの更新処理。
	stageRender_.Update();

	// 当たり判定。
	stagePhysics_.SetPosition(stagePos_);

	// コリジョン。
	pStageCollision_->SetPosition(stagePos_ + COLLISION_HEIGHT);
}

void Stage2::Render(RenderContext& rc)
{
	stageRender_.Draw(rc);
}

void Stage2::RotationFoolNewGO()
{
    std::vector<Vector3> RotFoolPosList =
	{
		Stage2GimmickPos::RotationFoolPosition::Pos1
	};

	for (size_t i = 0; i < RotFoolPosList.size(); i++)
	{
		auto rotFool = NewGO<RotationFool>(0, "rotationfool");
		rotFool->SetRotFoolPosition(RotFoolPosList[i]);
        rotFool->SetScale(scale_);
	}

}

