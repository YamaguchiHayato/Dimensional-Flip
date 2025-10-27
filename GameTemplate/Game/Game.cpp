#include "stdafx.h"
#include "Game.h"
#include "Fade.h"
#include "Src/WallActor.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Character/Enemy/EnemyBase.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "Src/Actor/Stage/Gimmick/JumpPad.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/UI/TimerUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/HPbarUI.h"
#include "Src/Scene/SceneManager.h"

namespace EnemyPosition
{
	const Vector3 Pos1(600.0f, 0.0f, 0.0f);
}

namespace GameParameter
{
	const float scale = 1.0f;

}

namespace WallPosition
{
	const Vector3 Pos1(0.0f, 0.0f, 25.0f);
	const Vector3 Pos2(300.0f, 0.0f, 25.0f);
	const Vector3 Pos3(900.0f, -20.0f, 23.0f);
	const Vector3 Pos4(1450.0f, -40.0f, 180.0f);
}

namespace StarPosition
{
	const Vector3 Pos1(1400.0f, 95.0f, -2000.0f);
}

namespace JumpPadPosition
{
	const Vector3 Pos1(1400.0f, -60.0f, -1680.0f);
}

namespace GoalPointPosition
{
	const Vector3 Pos1(1400.0f, 100.0f, -2000.0f);
}


Game::~Game()
{
	DeleteGO(player_);
	DeleteGO(stage1_);
	DeleteGO(timerUI_);
	DeleteGO(numberUI_);
	DeleteGO(scoreUI_);
	DeleteGO(hpbarUI_);
}

void Game::InitSkyCube()
{
	DeleteGO(skyCube_);
	SkyCube* m_SkyCube = NewGO<SkyCube>(0, "skycube");
	m_SkyCube->SetType(enSkyCubeType_Day);
	m_SkyCube->SetLuminance(0.5f);
	m_SkyCube->SetScale(700.0f);


	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_SkyCube->GetTextureFilePath(), 1.0f);
	// 環境日光の影響が分かりやすいように、ディレクションライトはオフに。
	g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);

}

bool Game::Start()
{
	UINewGO();

    FadeStart();

	InitSkyCube();

	// まず、"player"という名前のオブジェクトを探す
	player_ = FindGO<Player>("player");

	// もしプレイヤーが見つからなかった場合（nullptrだった場合）のみ、新しく生成する
	if (player_ == nullptr)
	{
		player_ = NewGO<Player>(0, "player");
	}
	stage1_ = NewGO<Stage1>(0, "stage1");


	WallNewGO();

	StarNewGO();

	JumpPadNewGO();

//	EnemyNewGO_Tracking();
//  PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{
}

void Game::WallNewGO()
{
	std::vector<Vector3> WallPosList =
	{
		WallPosition::Pos1,
		WallPosition::Pos2,
		WallPosition::Pos3,
		WallPosition::Pos4
	};

	for (size_t i = 0; i < WallPosList.size(); i++)
	{
		auto wall = NewGO<WallActor>(0, "wall");
		wall->SetWallPos(WallPosList[i]);

		if (i == 3) 
		{
			Quaternion wallRot;
			wallRot.SetRotationDegY(90.0f);
			wall->SetWallRot(wallRot);
		}
	}

}

void Game::JumpPadNewGO()
{
	std::vector<Vector3> JumpPadPosList =
	{
		JumpPadPosition::Pos1
	};

	for (size_t i = 0; i < JumpPadPosList.size(); i++)
	{
		auto jumppad = NewGO<JumpPad>(0, "jumppad");
		jumppad->SetJumpPadPosition(JumpPadPosList[i]);
	}
}

void Game::StarNewGO()
{
	std::vector<Vector3> StarPosList =
	{
		StarPosition::Pos1
	};

	for (size_t i = 0; i < StarPosList.size(); i++)
	{
		auto star = NewGO<Star>(0, "star");
		star->SetStarPosition(StarPosList[i]);
	}

}

void Game::UINewGO()
{
	TimerUINewGO();

	NumberUINewGO();

	ScoreUINewGO();

	HPbarUINewGO();
}

void Game::TimerUINewGO()
{
	timerUI_ = NewGO<TimerUI>(0, "timerUI");
}

void Game::NumberUINewGO()
{
	numberUI_ = NewGO<NumberUI>(0, "numberUI");
}

void Game::ScoreUINewGO()
{
	scoreUI_ = NewGO<ScoreUI>(0, "scoreUI");
}

void Game::HPbarUINewGO()
{
	hpbarUI_ = NewGO<HPbarUI>(0, "hpbarUI");
}

void Game::FadeStart()
{
    fade_ = FindGO<Fade>("fade");
    fade_->FadeTransition(FadeState::FadeStart);
}

void Game::EnemyNewGO_Tracking()
{
	trackingEnemy_ = NewGO<TrackingEnemy>(0, "TrackingEnemy");
	trackingEnemy_->enemyPosition_ = {EnemyPosition::Pos1};
	trackingEnemy_->enemyFP_ = trackingEnemy_->enemyPosition_;
}

