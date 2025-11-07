#include "stdafx.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"
// ギミック。
#include "Src/Actor/Stage/Gimmick/JumpPad.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/WallActor.h"

// UI。
#include "Src/UI/TimerUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/HPbarUI.h"

namespace
{
	const Vector3 SCALE(1.0f, 1.0f, 1.0f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

// namespace
namespace GimmickPos
{
    // スター。
    namespace StarPosition
    {
        const Vector3 Pos1(1500.0f, 95.0f, -2000.0f);
    }

    // ジャンプパッド。
    namespace JumpPadPosition
    {
        const Vector3 Pos1(1400.0f, -60.0f, -1800.0f);
    }

        // 透明壁。
    namespace WallPosition
    {
        const Vector3 Pos1(0.0f, 0.0f, 25.0f);
        const Vector3 Pos2(300.0f, 0.0f, 25.0f);
        const Vector3 Pos3(900.0f, -20.0f, 230.0f);
        const Vector3 Pos4(1450.0f, -40.0f, 180.0f);
    }

    // カメラトリガー。
    namespace TriggerPos
    {
        const Vector3 Pos1 = Vector3(200.0f, 0.0f, 0.0f);
        const Vector3 Pos2 = Vector3(1500.0f,-75.0f, 0.0f );
    }
}


Stage1::~Stage1()
{
    DeleteGO(pStageCollision_);
}

bool Stage1::Start()
{
    // 各UIの生成。
	UIInstance();


	const std::string stagePath = InitStage("Stage1/Stage1");
	stageRender_.Init(stagePath.c_str());

	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());
	stageRender_.SetScale(SCALE);

	// コリジョン。
	pStageCollision_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	pStageCollision_->CreateBox
	(   stagePos_ + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE );

	// 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;

	// コリジョンを破棄。
	pStageCollision_->SetIsEnableAutoDelete(false);

    // プレイヤー生成。
    PlayerNewGO();

    // カメラマネージャーの生成。
    pCameraManager_ = std::unique_ptr<CameraManager>
        (NewGO<CameraManager>(0, "cameramanager"));
    pPlayer_->InitCameraManager(pCameraManager_.get());


    // スター生成。
	StarNewGO();

    // ジャンプパッド生成。
	JumpPadNewGO();

    // 透明壁生成。
    WallNewGO();

    // カメラトリガー生成。
    DimensionTriggerNewGO();

	stageRender_.Update();
	return true;
}

void Stage1::Update()
{

	stageRender_.Update();

	// 当たり判定。
	stagePhysics_.SetPosition(stagePos_);

	// コリジョン。
	pStageCollision_->SetPosition(stagePos_ + COLLISION_HEIGHT);

}

void Stage1::Render(RenderContext& rc)
{
	stageRender_.Draw(rc);
}

void Stage1::JumpPadNewGO()
{
	std::vector<Vector3> JumpPadPosList =
	{
		GimmickPos::JumpPadPosition::Pos1
	};

	for (size_t i = 0; i < JumpPadPosList.size(); i++)
	{
		auto jumppad = NewGO<JumpPad>(0, "jumppad");
		jumppad->SetJumpPadPosition(JumpPadPosList[i]);
	}
}

void Stage1::StarNewGO()
{
	std::vector<Vector3> StarPosList =
	{
		GimmickPos::StarPosition::Pos1
	};

	for (size_t i = 0; i < StarPosList.size(); i++)
	{
		auto star = NewGO<Star>(0, "star");
		star->SetStarPosition(StarPosList[i]);
	}

}

void Stage1::WallNewGO()
{
	std::vector<Vector3> WallPosList =
	{
		GimmickPos::WallPosition::Pos1,
		GimmickPos::WallPosition::Pos2,
	//	WallPosition::Pos3,
	//	WallPosition::Pos4
	};

	for (size_t i = 0; i < WallPosList.size(); i++)
	{
		auto wall = NewGO<WallActor>(0, "wall");

		if (i == 3) 
		{
			Quaternion wallRot;
			wallRot.SetRotationDegY(90.0f);
			wall->SetWallRot(wallRot);
		}

        wall->SetWallPos(WallPosList[i]);

	}

}

void Stage1::DimensionTriggerNewGO()
{
	std::vector<Vector3> TriggerList =
	{
		GimmickPos::TriggerPos::Pos1,
		GimmickPos::TriggerPos::Pos2 
	};
	for (size_t i = 0; i < TriggerList.size(); i++)
	{
		auto trigger = NewGO<DimensionTrigger>(0, "dimensiontrigger");
		trigger->SetTriggerPos(TriggerList[i]);
	}

}

void Stage1::PlayerNewGO()
{
    pPlayer_ = NewGO<Player>(0, "player");
    pPlayer_->SetPlayerPos(Vector3(0.0f, 150.0f, 0.0f));
}

void Game::UIInstance()
{
	TimerInstance();

	NumberInstance();

	ScoreInstance();

	HPbarInstance();
}

void Game::TimerInstance()
{
    pTimerUI_ = NewGO<TimerUI>(0, "timerui");
}

void Game::NumberInstance()
{
    pNumberUI_ = NewGO<NumberUI>(0, "numberui");
}

void Game::ScoreInstance()
{
    pScoreUI_ = NewGO<ScoreUI>(0, "scoreui");
}

void Game::HPbarInstance()
{
    pHpbarUI_ = NewGO<HPbarUI>(0, "hpbarui");
}
