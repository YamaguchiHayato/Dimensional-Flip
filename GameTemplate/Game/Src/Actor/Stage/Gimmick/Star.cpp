#include "stdafx.h"
#include "Star.h"
#include "Src/Actor/Character/Player.h"
#include "GameClear.h"

namespace StarStatus
{
	const float STAR_ROTATE_SPEED = 3.0f;
	const Vector3 STAR_SCALE(0.5f, 0.5f, 0.5f);
	const Vector3 COLLISION_HEIGHT(0.0f, 25.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(365.0f, 5.0f, 225.0f);//コリジョンの大きさ
}

bool Star::Start()
{
	m_starRender.Init("Assets/stageMaterial/Stage1/Star.tkm");
	m_starRender.SetScale(StarStatus::STAR_SCALE);
	m_starRender.SetPosition(m_starPosition);

	m_player = FindGO<Player>("player");
	return true;
}

void Star::Update()
{
	/* 早期リターン。*/
	if (m_player == nullptr) { return; }

	Vector3 diff = m_player->GetPosition() - m_starPosition;
	if (diff.Length() <= 100.0f)
	{
		NewGO<GameClear>(0, "gameclear");
		DeleteGO(this);
	}

	/* 回転処理。*/
	Rotation();
	/* 座標をセット。*/
	m_starRender.SetPosition(m_starPosition);
	/* 描画更新。*/
	m_starRender.Update();
}

void Star::Rotation()
{
	/* 回転を計算数る処理。*/
	m_starRotation.AddRotationDegY(StarStatus::STAR_ROTATE_SPEED);

	/* 回転度数をセット。*/
	m_starRender.SetRotation(m_starRotation);
}

void Star::Render(RenderContext& rc)
{
	m_starRender.Draw(rc);
}