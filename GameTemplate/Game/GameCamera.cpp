#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Game.h"


namespace
{
	Vector3 CAMERA_TARGET;
}



//スタートメソッド
bool GameCamera::Start()
{
	////ニアクリップとファークリップの設定
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	//注視点からのベクトルの設定
	m_toCameraPos.Set(0.0f, 125.0f, -250.0f);
	m_player = FindGO<Player>("player");
	return true;
}

void GameCamera::Update()
{
	CameraMove();            // カメラの移動

	if (g_pad[0]->IsPress(enButtonB))
	{
		CameraSwitch();
	}

}

//void GameCamera::CameraStateTransition()
//{
//	switch (cameraState)
//	{
//	default:
//		break;
//	}
//}

void GameCamera::CameraMove()
{

	CAMERA_TARGET = m_player->m_position;

	CAMERA_TARGET.y += 85.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);

	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);
	//キャラクターを斜め上から見る視点にする
	//Vector3 toPos = { 0.0f, 50.0f, -300.0f };
	//Vector3 pos = target + toPos;
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.2f) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する。
	Vector3 pos = CAMERA_TARGET + m_toCameraPos;
	//m_toCameraPos = pos;
	//メインカメラに注視点と視点を設定する
	g_camera3D->SetTarget(CAMERA_TARGET);
	g_camera3D->SetPosition(pos);

	//カメラの更新
	g_camera3D->Update();
}

void GameCamera::CameraSwitch()
{
	if (g_pad[0]->IsPress(enButtonB))
	{
		// ステートが2Dモードの時
		if (cameraMode == mode_2D)
		{
			cameraMode = mode_3D;
			SwitchTo3DMode();
		}

		else
		{
			cameraMode = mode_2D;
			SwitchTo2DMode();
		}

	}
}

void GameCamera::SwitchTo2DMode()
{
    // 正射影投影（Orthographic）に切り替え
    g_camera3D->SetProjectionOrthographic(true,1280.0f,720.0f,1.0f,10000.0f,60.0f, 16.0f / 9.0f);

    // カメラ位置をXY固定、Z奥に設定（例: プレイヤーの真上から）
    if (m_player) {
        Vector3 playerPos = m_player->m_position;
        // XYはプレイヤーに合わせ、Zは固定値（例: -500.0f など）
        Vector3 camPos(playerPos.x, playerPos.y, -500.0f);
        g_camera3D->SetPosition(camPos);
        g_camera3D->SetTarget(Vector3(playerPos.x, playerPos.y, 0.0f));
    }

    // 回転は0度（カメラの向きを固定）
    // プレイヤー移動はXYのみ（移動制限はプレイヤー側で制御）

    g_camera3D->Update();
}

void GameCamera::SwitchTo3DMode()
{
    // 透視投影（Perspective）に切り替え
    g_camera3D->SetProjectionOrthographic(false,1280.0f, 720.0f, 1.0f, 10000.0f, 60.0f, 16.0f / 9.0f);

    // カメラ位置を「上から斜め後ろ」に設定
    if (m_player) {
        Vector3 playerPos = m_player->m_position;
        // 斜め後ろ上からプレイヤーを見る（例: Y+上、Z-奥）
        Vector3 camOffset(0.0f, 125.0f, -250.0f);
        Vector3 camPos = playerPos + camOffset;
        g_camera3D->SetPosition(camPos);
        g_camera3D->SetTarget(playerPos);
    }

    // 回転はプレイヤー方向（既存のカメラ回転処理を利用）
    // プレイヤー移動はXYZ自由（移動制限解除はプレイヤー側で制御）

    g_camera3D->Update();
}

