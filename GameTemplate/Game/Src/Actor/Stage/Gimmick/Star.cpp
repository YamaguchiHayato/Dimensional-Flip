#include "stdafx.h"
#include "Star.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Direction/StageClear.h"
#include "Src/Direction/CutIn/CutInView.h"

namespace StarStatus
{
	const float STAR_ROTATE_SPEED = 3.0f;
	const Vector3 STAR_SCALE(0.5f, 0.5f, 0.5f);
	const Vector3 COLLISION_HEIGHT(0.0f, 25.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(365.0f, 5.0f, 225.0f);//コリジョンの大きさ
}

bool Star::Start()
{
	m_starRender.Init("Assets/stage/Stage1/Star.tkm");
	m_starRender.SetScale(StarStatus::STAR_SCALE);
	m_starRender.SetPosition(m_starPosition);

	m_player = FindGO<Player>("player");

	return true;
}

void Star::Update()
{
	if (m_player == nullptr) { return; }

	Vector3 diff = m_player->GetPlayerPos() - m_starPosition;
	if (diff.Length() <= 100.0f)
	{
        NewGO<StageClear>(0, "stageclear");
		DeleteGO(this);
        //NewGO<app::cutIn::CutInView>(0, "CutInView");
        //DeleteGO(this);
	}

	Rotation();
	m_starRender.SetPosition(m_starPosition);
	m_starRender.Update();
}

void Star::Rotation()
{
	m_starRotation.AddRotationDegY(StarStatus::STAR_ROTATE_SPEED);

	m_starRender.SetRotation(m_starRotation);
}

void Star::Render(RenderContext& rc)
{
	m_starRender.Draw(rc);
}
