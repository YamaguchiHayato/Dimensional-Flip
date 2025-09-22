#include "stdafx.h"
#include "Game.h"
#include "Src/GameCamera.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Character/Enemy/EnemyBase.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"

namespace EnemyPosition
{
	const Vector3 pos(600.0f, 0.0f, 0.0f);
}

namespace GameParameter
{
	const float scale = 1.0f;

}


/* カメラの視点を回転できる座標上限を設定するパラメータ。 */
namespace CameraParameter
{
	/* @param CAMERA_ROT_MIN_X : カメラのX軸回転の最小値。*/
	const float CAMERA_ROT_MIN_X = 0;
	/* @param CAMERA_ROT_MAX_X : カメラのX軸回転の最大値。*/
	const float CAMERA_ROT_MAX_X = 10000;
	/* @param CAMERA_ROT_MIN_Z : カメラのZ軸回転の最小値。*/
	const float CAMERA_ROT_MIN_Z = 0;
	/* @param CAMERA_ROT_MAX_Z : カメラのZ軸回転の最大値。*/
	const float CAMERA_ROT_MAX_Z = 10000;
}

bool Game::Start()
{
    m_player = NewGO<Player>(0, "player");
	m_stage1 = NewGO<Stage1>(0, "stage1");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	auto camera = FindGO<GameCamera>("gamecamera");
	camera->ClearOrbitZones();

	/* カメラの視点を回転を可能にするエリア登録座標。*/
	camera->AddOrbitZoneXZ
	(
		CameraParameter::CAMERA_ROT_MIN_X, 
		CameraParameter::CAMERA_ROT_MAX_X,
		CameraParameter::CAMERA_ROT_MIN_Z,
		CameraParameter::CAMERA_ROT_MAX_Z
	);

	// さらに別の小部屋
//	camera->AddOrbitZoneXZ(450.0f, 560.0f, -20.0f, 80.0f);

//	EnemyNewGO_Tracking();
//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{
//	FontRenderDebug();
}

void Game::EnemyNewGO_Tracking()
{
	m_trackingEnemy = NewGO<TrackingEnemy>(0, "TrackingEnemy");
	m_trackingEnemy->m_EnemyPosition = {EnemyPosition::pos};
	m_trackingEnemy->m_EnemyFP = m_trackingEnemy->m_EnemyPosition;
}

void Game::FontRenderDebug()
{
	Vector3 p = m_player ? m_player->GetPosition() : Vector3::Zero;
	wchar_t buf[128];
	swprintf_s(buf, L"Player: X=%.2f  Y=%.2f  Z=%.2f", p.x, p.y, p.z);

	m_fontRender.SetText(buf);
	m_fontRender.SetScale(1.5f);                 // ← 見えるサイズ
	m_fontRender.SetPosition({ 20.0f, 20.0f, 0 }); // ← 画面左上
	m_fontRender.SetColor(g_vec4White);
}

void Game::Render(RenderContext rc)
{
	m_fontRender.Draw(rc);
}