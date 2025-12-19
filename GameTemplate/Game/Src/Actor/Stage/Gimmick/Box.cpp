#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/Box.h"
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
    std::string foolPath = InitGimmick("Stage1/Block");
    render_.Init(foolPath.c_str(), 0, 0, enModelUpAxisZ, false, true);// モデルをセットする。
	
	// モデルの大きさ。
	render_.SetScale(SCALE);

	// モデルの更新作業。
	render_.Update();

	// 探索処理。
	pPlayer = FindGO<Player>("player");

    gimmickPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());

	// コリジョン。
	pGimmickCollision_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	pGimmickCollision_->CreateBox(gimmickPos_ + GIMMICKCOLLISION,Quaternion::Identity, GIMMICKCOLLISION);

	// 座標を設定。
	render_.SetPosition(gimmickPos_);
	// コリジョンを破棄。
	pGimmickCollision_->SetIsEnableAutoDelete(false);

    return true;
}

void Box::Update()
{
    // ギミックモデルの更新。
    render_.Update();
    // ギミックモデルの座標更新。
    render_.SetPosition(gimmickPos_);
    // ギミックの座標設定。
    gimmickPhysics_.SetPosition(gimmickPos_);
    // ギミックコリジョンの設定。
    pGimmickCollision_->SetPosition(gimmickPos_ + GIMMICKCOLLISION);
}

void Box::Render(RenderContext& rc)
{
    render_.Draw(rc);
}
