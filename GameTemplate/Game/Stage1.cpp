#include "stdafx.h"
#include "Stage1.h"

namespace
{
	const Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

Stage1::~Stage1()
{
	DeleteGO(m_collisionObject);
}

bool Stage1::Start()
{
	modelRender.Init("Assets/stage&material/stage2VerTest.tkm");
	m_physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());

	// コリジョン。
	m_collisionObject = NewGO<CollisionObject>(0, "collisionobject");


	//コリジョンを動く床に設置
	m_collisionObject->CreateBox
	(
		m_position + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE
	);

	// コリジョンを破棄。
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void Stage1::Update()
{
	modelRender.SetScale(SCALE);
	modelRender.Update();

	// 当たり判定。
	m_physicsStaticObject.SetPosition(m_position);

	// コリジョン。
	m_collisionObject->SetPosition(m_position + COLLISION_HEIGHT);

}

void Stage1::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}