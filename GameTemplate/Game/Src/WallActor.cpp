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
	/* IniTranslucent 半透明用のInit関数。*/
	m_WallRender.IniTranslucent("Assets/stageMaterial/Stage1/Wall.tkm");
	/* モデル大きさの設定。*/
	m_WallRender.SetScale(WallStatus::SCALE);
	/* モデルの座標の設定。*/
	m_WallRender.SetPosition(m_WallPos);
	/* モデルの回転の設定。*/ 
	m_WallRender.SetRotation(m_WallRot);
	/* モデルの更新。*/
	m_WallRender.Update();
	/* モデルを透明に設定。*/
	m_WallRender.SetAlpha(0.0f);
	/*コリジョンの当たり判定を生成。*/
	m_physicsStaticObject.CreateFromModel(m_WallRender.GetModel(), m_WallRender.GetModel().GetWorldMatrix());
	/* コリジョンをnewする。*/
	m_Collision = NewGO<CollisionObject>(0);
	/* コリジョンを生成。*/
	m_Collision->CreateBox
	(
		m_WallPos+ WallStatus::COLLISION_HEIGHT,
		m_WallRot,
		WallStatus::COLLISION_SIZE
	);
	/*コリジョンの破棄。*/
	m_Collision->SetIsEnableAutoDelete(false);

	return true;
}

void WallActor::Update()
{
	/* モデルの座標を更新。*/
	m_WallRender.SetPosition(m_WallPos);
	/* モデルの透明度を更新。*/
	m_WallRender.SetAlpha(0.0f);
	/* モデルの回転度数を更新。*/
	m_WallRender.SetRotation(m_WallRot);
	/* モデルの更新。*/
	m_WallRender.Update();

}

void WallActor::Render(RenderContext& rc)
{
//	m_WallRender.Draw(rc);
}
