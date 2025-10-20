#include "stdafx.h"
#include "WallActor.h"

namespace WallStatus
{
	const Vector3 SCALE(10.0f, 10.0f, 10.0f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);
}


WallActor::~WallActor()
{
	DeleteGO(m_Collision);
}

bool WallActor::Start()
{
	m_WallRender.IniTranslucent("Assets/stage/Stage1/Wall.tkm");
	m_WallRender.SetScale(WallStatus::SCALE);
	m_WallRender.SetPosition(m_WallPos);
	m_WallRender.SetRotation(m_WallRot);
	m_WallRender.Update();
	m_WallRender.SetAlpha(0.0f);
	m_physicsStaticObject.CreateFromModel(m_WallRender.GetModel(), m_WallRender.GetModel().GetWorldMatrix());
	m_Collision = NewGO<CollisionObject>(0);

    m_Collision->CreateBox
	(
		m_WallPos+ WallStatus::COLLISION_HEIGHT,
		m_WallRot,
		WallStatus::COLLISION_SIZE
	);
	m_Collision->SetIsEnableAutoDelete(false);

	return true;
}

void WallActor::Update()
{
	m_WallRender.SetPosition(m_WallPos);
	m_WallRender.SetAlpha(0.0f);
	m_WallRender.SetRotation(m_WallRot);
	m_WallRender.Update();
}
