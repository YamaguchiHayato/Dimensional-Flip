#include "stdafx.h"
#include "DirectXMath.h"
#include "GameCamera.h"
#include "Player.h"
#include "Game.h"

namespace
{
	Vector3 CAMERA_TARGET;

	// カメラの各種パラメータ。
	const float CAMERA_WIDHT = 1280.0f;       // 画面幅。
	const float CAMERA_HEIGHT = 720.0f;       // 画面高さ。
	const float CAMERA_NEAR_Z = 1.0f;         // ニアクリップ。
	const float CAMERA_FAR_Z = 10000.0f;      // ファークリップ。
	const float CAMERA_FOV_Y = 60.0f;         // 画角(縦)。
	const float CAMERA_ASPECT = 16.0f / 9.0f; // アスペクト比。
}

inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	return a + (b - a) * t;
}

/// <summary>
/// 初期化処理。
/// </summary>
bool GameCamera::Start()
{
	////ニアクリップとファークリップの設定
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	//注視点からのベクトルの設定
	m_player = FindGO<Player>("player");
	m_toCameraPos.Set(m_player->GetPosition().x, m_player->GetPosition().y, -250.0f);

	return true;

}

/// <summary>
///	更新処理。
/// </summary>
void GameCamera::Update()
{
	CameraSwitch(); // カメラの視点切替
	CameraMove();   // カメラの移動。
}

/// <summary>
/// カメラの追従。
/// </summary>
void GameCamera::CameraMove()
{
	// 早期リターン。
	if (!m_player) return;

	// カメラの追従設定。
	Vector3 playerPos = m_player->m_position;
	Vector3 playerForward = m_player->GetPosition();

	// 背後オフセットを動的に算出。
	Vector3 behind = playerPos - playerForward * followDistance_;
	behind.y += verticalOffset_;

	Vector3 desiredPosition = behind;
	Vector3 desiredTarget = playerPos;


	//// カメラが2Dモードの場合。
	//if (cameraMode == mode_2D ? false : true)
	//{
	//	float t = 1.0f - std::exp(-followLerp_ * g_gameTime->GetFrameDeltaTime());
	//	currentPosition = Lerp(currentPosition, desiredPosition, t);
	//	currentTarget = Lerp(currentTarget, desiredTarget, t);
	//	g_camera3D->SetPosition(currentPosition);
	//	g_camera3D->SetTarget(currentTarget);		//// 横からに視点を固定させる。
	//	//Vector3 camOffset(0.0f, 0.0f, -250.0f);
	//	//Vector3 camPos = playerPos + camOffset;
	//	//Vector3 camTarget = playerPos;
	//	//g_camera3D->SetPosition(camPos );
	//	//g_camera3D->SetTarget(camTarget);

	//}

	//// カメラが3Dモードの場合。
	//else
	//{
	//	// 真後ろからの追従視点。
	//	Vector3 camPos(playerPos.x - 1000.0f, playerPos.y, playerPos.z);
	//	Vector3 camTarget(playerPos.x, playerPos.y, playerPos.z);
	//	g_camera3D->SetPosition(camPos);
	//	g_camera3D->SetTarget(camTarget);
	//}

	g_camera3D->Update();
}


/// <summary>
/// カメラの視点切替。
/// </summary>
void GameCamera::CameraSwitch()
{
	
	CameraMode requestCameraMode = CameraMode::None;

	// Bボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		// ステートが2Dモードの時
		if (cameraMode == mode_2D)
		{
			// 3Dモードに切り替えをリクエスト
			requestCameraMode = mode_3D;
		}

		// ステートが3Dモードの時
		else
		{    
			// 2Dモードに切り替えをリクエスト
			requestCameraMode = mode_2D;
		}
	}

	// リクエストがNoneでなければ
	if (requestCameraMode != CameraMode::None)
	{
		// 現在のカメラモードとリクエストされたモードが異なれば
		if (requestCameraMode != cameraMode)
		{
			// カメラモードを切り替え
			switch (requestCameraMode)
			{
				 // 2Dモードに切り替え
			case GameCamera::mode_3D:
				SwitchTo2DMode();
				break;

				// 3Dモードに切り替え
			case GameCamera::mode_2D:
				SwitchTo3DMode();
				break;
			}

			// カメラモードを更新
			cameraMode = requestCameraMode;
		}
	}

}

/// <summary>
/// カメラの2D視点用。
/// </summary>
void GameCamera::SwitchTo2DMode()
{

	// 正射影投影（Orthographic）に切り替え
	 g_camera3D->SetProjectionOrthographic(true, CAMERA_WIDHT, CAMERA_HEIGHT, CAMERA_NEAR_Z, CAMERA_FAR_Z, CAMERA_FOV_Y, CAMERA_ASPECT);


	if (m_player) {
		m_playerPos = m_player->m_position;

		// 横スクロール風の視点（Z軸固定）
		Vector3 camPos(m_playerPos.x - 1000.0f, m_playerPos.y + 100.0f, 0.0f);
		Vector3 camTarget(m_playerPos.x, m_playerPos.y, 0.0f);
		g_camera3D->SetPosition(camPos);
		g_camera3D->SetTarget(camTarget);
	}
	g_camera3D->Update();
}

/// <summary>
/// カメラの3D視点用。
/// </summary>
void GameCamera::SwitchTo3DMode()
{
	if (m_player) {


		Vector3 playerPos = m_player->m_position;
		Vector3 camOffset(0.0f, 125.0f, -250.0f);
		Vector3 camPos = playerPos + camOffset;

		// 
		m_toCameraPos = camPos;
		m_CameraTarget = playerPos;

		g_camera3D->SetPosition(camPos);
		g_camera3D->SetTarget(playerPos);

	}

	g_camera3D->Update();
}

