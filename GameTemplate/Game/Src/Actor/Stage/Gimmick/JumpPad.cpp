#include "stdafx.h"
#include "JumpPad.h"
#include "Src/Actor/Character/Player.h"

namespace
{
	const float JUMPPAD_JUMP_POWER = 50.0f;
	const Vector3 JUMPPAD_SCALE(0.2f, 0.2f, 0.2f);
	const Vector3 JUMPPAD_POSITION(500.0f, -20.0f, 0.0f);

	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(365.0f, 5.0f, 225.0f);//コリジョンの大きさ
}

JumpPad::~JumpPad()
{
	DeleteGO(m_jumpPadCollision);
}

bool JumpPad::Start()
{
	/* JumpPadのモデルを設定。*/
	m_jumpPadModelRender.Init("Assets/stageMaterial/Stage1/JumpPad.tkm");
	// モデルの大きさ。
	m_jumpPadModelRender.SetScale(JUMPPAD_SCALE);

	// モデルの更新作業。
	m_jumpPadModelRender.Update();

	// 探索処理。
	m_player = FindGO<Player>("player");

	// 当たり判定。
	m_jumpPadPhysics.CreateFromModel
	(
		m_jumpPadModelRender.GetModel(),
		m_jumpPadModelRender.GetModel().GetWorldMatrix()
	);

	// コリジョン。
	m_jumpPadCollision = NewGO<CollisionObject>(0, "collisionobject");


	//コリジョンを動く床に設置
	m_jumpPadCollision->CreateBox
	(
		m_jumpPadPosition + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE
	);

	// 座標を設定。
	m_jumpPadModelRender.SetPosition(m_jumpPadPosition);

	// コリジョンを破棄。
	m_jumpPadCollision->SetIsEnableAutoDelete(false);
	m_padFirstPosition = m_jumpPadPosition;
	return true;
}

void JumpPad::Update()
{
	/* 早期リターン。*/
	if (m_player == nullptr) return;

	/* JumpPad処理。*/
	LaunchPlayer();
	/* バレットコリジョンの適用。*/
	m_jumpPadPhysics.SetPosition(m_jumpPadPosition);

	// コリジョン。
	m_jumpPadCollision->SetPosition(m_jumpPadPosition + COLLISION_HEIGHT);

	// モデルの更新処理。
	m_jumpPadModelRender.Update();

}

void JumpPad::LaunchPlayer()
{
	/* プレイヤーがジャンプパッドの上にいるか? */
	if (m_jumpPadCollision->IsHit(m_player->GetCharacterController()))
	{
		m_player->moveSpeed_.y = 400.0f;
	}

	/* 上に乗っていなければ何もしない。*/
	else { return; }
}

void JumpPad::Render(RenderContext& rc)
{
	m_jumpPadModelRender.Draw(rc);
}

