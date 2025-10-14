#include "stdafx.h"
#include "Src/Camera/FollowCamera.h"
#include "Src/Camera/GameCamera.h"
#include "Src/Actor/Character/Player.h"
//void FollowCamera::Update(GameCamera* owner)
//{
//    if (!g_camera3D) return;
//
//    nsK2EngineLow::Vector3 playerPos = owner->QueryPlayerPos();
//    nsK2EngineLow::Vector3 camPos = playerPos + owner->GetFollowOffset();
//    g_camera3D->SetTarget(playerPos);
//    g_camera3D->SetPosition(camPos);
//
//    owner->SetFollowOffset(camPos - playerPos);
//}

bool FollowCamera::Start()
{
	////ニアクリップとファークリップの設定
	g_camera3D->SetNear(10.0f);         
	// スカイキューブの大きさに合わせて調整
	g_camera3D->SetFar(700.0f);      
	//注視点からのベクトルの設定
	m_CameraPos.Set(0.0f, 35.0f, -300.0f);
	m_player = FindGO<Player>("player");
	return true;
}

void FollowCamera::Update()
{
	if (!m_player) return;

	Move();
}

void FollowCamera::Move()
{
	//カメラの更新
	//注視点の計算
	Vector3 target = m_player->PlayerPos_;

	target.y += 85.0f;

	Vector3 toCameraPosOld = m_CameraPos;
	//Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_CameraPos);

	axisX.Cross(Vector3::AxisY, m_CameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_CameraPos);
	//キャラクターを斜め上から見る視点にする
	//Vector3 toPos = { 0.0f, 50.0f, -300.0f };
	//Vector3 pos = target + toPos;
	Vector3 toPosDir = m_CameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.2f) {
		//カメラが上向きすぎ。
		m_CameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ。
		m_CameraPos = toCameraPosOld;
	}

	//視点を計算する。
	Vector3 pos = target + m_CameraPos;
	//m_toCameraPos = pos;
	//メインカメラに注視点と視点を設定する
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新
	g_camera3D->Update();

}