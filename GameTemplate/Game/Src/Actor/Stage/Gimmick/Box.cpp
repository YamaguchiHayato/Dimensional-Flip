#include "stdafx.h"
#include "Box.h"
#include "Src/Actor/Character/Player.h"

namespace
{
    const Vector3 GIMMICKCOLLISION(0.0f, 0.0f, 0.0f);//コリジョンの高さ
	const Vector3 SCALE(2.0f, 2.0f, 2.0f);	//モデルの大きさ
}

Box::~Box()
{
    if (pGimmickCollision_)
    {
        DeleteGO(pGimmickCollision_);
        pGimmickCollision_ = nullptr;
    }
}

bool Box::Start()
{
    	//ファイルパス
    std::string foolPath = InitGimmick("Stage2/Box");
    gimmickRender_.Init(foolPath.c_str(), 0, 0, enModelUpAxisZ, false, true);// モデルをセットする。
	
	// モデルの大きさ。
	gimmickRender_.SetScale(SCALE);

	// モデルの更新作業。
	gimmickRender_.Update();

	// 探索処理。
	pPlayer = FindGO<Player>("player");

    gimmickPhysics_.CreateFromModel(gimmickRender_.GetModel(), gimmickRender_.GetModel().GetWorldMatrix());

	// コリジョン。
	pGimmickCollision_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	pGimmickCollision_->CreateBox(gimmickPos_ + GIMMICKCOLLISION,Quaternion::Identity, GIMMICKCOLLISION);

	// 座標を設定。
	gimmickRender_.SetPosition(gimmickPos_);
	// コリジョンを破棄。
	pGimmickCollision_->SetIsEnableAutoDelete(false);

    return true;
}

void Box::Update()
{
    // ギミックモデルの更新。
    gimmickRender_.Update();
    // ギミックモデルの座標更新。
    gimmickRender_.SetPosition(gimmickPos_);
    // ギミックの座標設定。
    gimmickPhysics_.SetPosition(gimmickPos_);
    // ギミックコリジョンの設定。
    pGimmickCollision_->SetPosition(gimmickPos_ + GIMMICKCOLLISION);
}

void Box::Render(RenderContext& rc)
{
    gimmickRender_.Draw(rc);
}
