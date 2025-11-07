#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace
{
    const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE (365.0f, 5.0f, 225.0f);//コリジョンの大きさ
	const Vector3 SCALE(3.0f, 3.0f, 5.0f);	//モデルの大きさ

}

namespace GimmickPos
{
    // 回転トリック。
    namespace RotationFoolPosition
    {
        const Vector3 Pos1(300.0f, 0.0f, -0.0f);
    }
}

bool RotationFool::Start()
{
	//ファイルパス
    std::string foolPath = InitGimmick("Stage2/RotationFool");
    gimmickRender_.Init(foolPath.c_str(), 0, 0, enModelUpAxisZ, false, true);// モデルをセットする。
	
	// モデルの大きさ。
	gimmickRender_.SetScale(SCALE);

	// モデルの更新作業。
	gimmickRender_.Update();

	// 探索処理。
	pPlayer = FindGO<Player>("player");

	// 当たり判定。
    gimmickPhysics_.CreateFromModel(gimmickRender_.GetModel(), gimmickRender_.GetModel().GetWorldMatrix());

	// コリジョン。
	pGimmickCollision_ = NewGO<CollisionObject>(0, "collisionobject");


	//コリジョンを動く床に設置
	pGimmickCollision_->CreateBox
	(
		gimmickPos_ + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE
	);

	// 座標を設定。
	gimmickRender_.SetPosition(gimmickPos_);
    initGimmickPos_ = gimmickPos_;

	// コリジョンを破棄。
	pGimmickCollision_->SetIsEnableAutoDelete(false);
    return true;
}

void RotationFool::Update()
{
    // ギミックの更新。
    gimmickRender_.Update();
    // ギミックの物理設定。
    gimmickPhysics_.SetPosition(gimmickPos_);
    // ギミックコリジョンの設定。
    pGimmickCollision_->SetPosition(gimmickPos_ + collisonHeight_);
}

void RotationFool::Render(RenderContext& rc)
{
    gimmickRender_.Draw(rc);
}

