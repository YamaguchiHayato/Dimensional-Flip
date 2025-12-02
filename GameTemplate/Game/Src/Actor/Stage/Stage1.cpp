#include "stdafx.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Core/CameraManager.h"
// ギミック。
#include "Src/Actor/Stage/Gimmick/JumpPad.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/WallActor.h"


namespace
{
	const Vector3 SCALE(1.0f, 1.0f, 1.0f);
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

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
        const Vector3 Pos3 = Vector3(1400.0f, -60.0f, -1800.0f);
    }
}

Stage1::~Stage1()
{
    for (auto* p : lJumpPad_)
    {
        DeleteGO(p);
    }


    for (auto* p : lStar_)
    {
        DeleteGO(p);
    }

    for (auto* p : lWall_)
    {
        DeleteGO(p);
    }

    for (auto* p : lDimensionTrigger_)
    {
        DeleteGO(p);
    }

    lJumpPad_.clear();
    lStar_.clear();
    lWall_.clear();
    lDimensionTrigger_.clear();
}

bool Stage1::Start()
{

	const std::string stagePath = InitStage("Stage1/Stage1");
	stageRender_.Init(stagePath.c_str());

	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());
	stageRender_.SetScale(SCALE);

	// 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;


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
		pJumpPad_= NewGO<JumpPad>(0, "jumppad");
        pJumpPad_->SetJumpPadPosition(JumpPadPosList[i]);
        lJumpPad_.push_back(pJumpPad_);
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
        lStar_.push_back(star);
	}

}

void Stage1::WallNewGO()
{
	std::vector<Vector3> WallPosList =
	{
		GimmickPos::WallPosition::Pos1,
		GimmickPos::WallPosition::Pos2,
	};

	for (size_t i = 0; i < WallPosList.size(); i++)
	{
       
		pWall_ = NewGO<WallActor>(0, "wall");

		if (i == 3) 
		{
			Quaternion wallRot;
			wallRot.SetRotationDegY(90.0f);
			pWall_->SetWallRot(wallRot);
		}

        pWall_->SetWallPos(WallPosList[i]);
        lWall_.push_back(pWall_);
	}

}

void Stage1::DimensionTriggerNewGO()
{
	std::vector<Vector3> TriggerList =
	{
		GimmickPos::TriggerPos::Pos1,
		GimmickPos::TriggerPos::Pos2,
	};
	for (size_t i = 0; i < TriggerList.size(); i++)
	{
		pDimensionTrigger_ = NewGO<DimensionTrigger>(0, "dimensiontrigger");
		pDimensionTrigger_->SetTriggerPos(TriggerList[i]);
        lDimensionTrigger_.push_back(pDimensionTrigger_);
	}

}


