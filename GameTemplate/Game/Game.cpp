#include "stdafx.h"
#include "Game.h"
#include "Src/Camera/GameCamera.h"
#include "Src/WallActor.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Character/Enemy/EnemyBase.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "OrbitCameraStrategy.h"

namespace EnemyPosition
{
	const Vector3 pos(600.0f, 0.0f, 0.0f);
}

namespace GameParameter
{
	const float scale = 1.0f;

}

namespace WallPosition
{
	const Vector3 Pos1(0.0f, 0.0f, 25.0f);
	const Vector3 Pos2(300.0f, 0.0f, 25.0f);
	const Vector3 Pos3(900.0f, -20.0f, 25.0f);
	const Vector3 Pos4(1350.0f, -40.0f, 0.0f);
	
}


/* カメラの視点を回転できる座標上限を設定するパラメータ。 */
namespace CameraParameter
{
	/* 許可領域1。*/
	namespace Territory1
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

	/* 許可領域2。*/ 
	namespace Territory2
	{
		/* @param CAMERA_ROT_MIN_X : カメラのX軸回転の最小値。*/
		const float CAMERA_ROT_MIN_X = 10001;
		/* @param CAMERA_ROT_MAX_X : カメラのX軸回転の最大値。*/
		const float CAMERA_ROT_MAX_X = 20000;
		/* @param CAMERA_ROT_MIN_Z : カメラのZ軸回転の最小値。*/
		const float CAMERA_ROT_MIN_Z = 10001;
		/* @param CAMERA_ROT_MAX_Z : カメラのZ軸回転の最大値。*/
		const float CAMERA_ROT_MAX_Z = 20001;

	}
}

bool Game::Start()
{
    m_player = NewGO<Player>(0, "player");
	m_stage1 = NewGO<Stage1>(0, "stage1");

	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	auto strategy = std::make_unique<OrbitCameraStrategy>(-45.0f, 0.3f, OrbitMode::XZ);
	m_gameCamera->SetStrategy(std::move(strategy));
	auto camera = FindGO<GameCamera>("gamecamera");

	camera->ClearOrbitZones();

	/* ゾーン登録。*/
	SetupViewRotationAreas();

	/* 透明壁をNewGO。*/
	WallNewGO();

//	EnemyNewGO_Tracking();
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{
	/* XZ方向への回転アクション。*/
	if (g_pad[0]->IsTrigger(enButtonX)) {
		auto orbit = dynamic_cast<OrbitCameraStrategy*>(m_gameCamera->GetStrategy());
		if (orbit) orbit->SetMode(OrbitMode::XZ);
	}

	/* YZ方向への回転アクション。*/ 
	if (g_pad[0]->IsTrigger(enButtonY)) {
		auto orbit = dynamic_cast<OrbitCameraStrategy*>(m_gameCamera->GetStrategy());
		if (orbit) orbit->SetMode(OrbitMode::YZ);
	}
}

void Game::WallNewGO()
{
	/* 座標を指定してモデルを複数描画*/
	std::vector<Vector3> WallPosList =
	{
		WallPosition::Pos1,
		WallPosition::Pos2,
		WallPosition::Pos3,
		WallPosition::Pos4
	};

	//for (auto& m_pos : WallPosList)
	//{
	//	auto wall = NewGO<WallActor>(0, "wall");
	//	wall->SetWallPos(m_pos);


	//}

	for (size_t i = 0; i < WallPosList.size(); i++)
	{
		auto wall = NewGO<WallActor>(0, "wall");
		wall->SetWallPos(WallPosList[i]);

		if (i == 3) /* Pos4*/
		{
			Quaternion wallRot;
			wallRot.SetRotationDegY(90.0f);
			wall->SetWallRot(wallRot);
		}
	}

}

void Game::EnemyNewGO_Tracking()
{
	m_trackingEnemy = NewGO<TrackingEnemy>(0, "TrackingEnemy");
	m_trackingEnemy->m_EnemyPosition = {EnemyPosition::pos};
	m_trackingEnemy->m_EnemyFP = m_trackingEnemy->m_EnemyPosition;
}

void Game::SetupViewRotationAreas()
{
	/* 1つ目の許可領域。*/
	m_gameCamera->AddOrbitZoneXZ
	(
		CameraParameter::Territory1::CAMERA_ROT_MIN_X,
		CameraParameter::Territory1::CAMERA_ROT_MAX_X,
		CameraParameter::Territory1::CAMERA_ROT_MIN_Z,
		CameraParameter::Territory1::CAMERA_ROT_MAX_Z
	);

	/* 2つ目の許可領域。*/
	m_gameCamera->AddOrbitZoneXZ
	(
		CameraParameter::Territory2::CAMERA_ROT_MIN_X,
		CameraParameter::Territory2::CAMERA_ROT_MAX_X,
		CameraParameter::Territory2::CAMERA_ROT_MIN_Z,
		CameraParameter::Territory2::CAMERA_ROT_MAX_Z
	);

}