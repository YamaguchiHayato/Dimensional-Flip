#include "stdafx.h"
#include "Src/Actor/Stage/Stage3.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Actor/Character/Player.h"
#include "Src/WallActor.h"
#include "Src/Actor/Character/Player.h"

namespace nsStage3
{
    namespace nsTrigger
    {
        const Vector3 Pos1(1600.0f, 0.0f, 45.0f);
        const Vector3 Pos2(1600.0f, 0.0f, -2200.0f);
        const Vector3 Pos3(3600.0f, 0.0f, -2200.0f);
        // ボス部屋前。
        const Vector3 Pos4(3600.0, 39.2f, -3927.0f);
    }

    namespace nsWall
    {
        // 透明壁設置座標。
        // X軸方向に設置する座標。
        const Vector3 Pos1(700.0f,  0.0f, 70.0f);   // 0
        const Vector3 Pos2(1000.0f, 0.0f, 70.0f);   // 1

        // Z軸方向に設置する座標。
        const Vector3 Pos3(1600.0f, 0.0f, -500.0f); // 2
        const Vector3 Pos4(1600.0f, 0.0f, -700.0f); // 3
    }

    namespace nsBoss
    {
        const Vector3 InitPos(3600.0, 39.2f, -4300.0f);
    }
}

//Stage3::~Stage3()
//{
//    for (WallActor* pWall : pWallActorList_)
//    {
//        DeleteGO(pWall);
//    }
//}

bool Stage3::Start()
{
	const std::string stagePath = InitStage("Stage3/stage3");
	stageRender_.Init(stagePath.c_str());

	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());

    // 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;

    // 大きさ設定。
    stageRender_.SetScale(Vector3::One);
    stageRender_.Update();

    BossInstance();
    TriggerInstace();
    WallActorInstace();

    pPlayer_ = FindGO<Player>("player");
    return true;
}

void Stage3::Update()
{
    stageRender_.SetScale(Vector3::One);
    stageRender_.SetPosition(stagePos_);
    // モデルの更新処理。
    stageRender_.Update();
}


void Stage3::Render(RenderContext& rc)
{
    stageRender_.Draw(rc);
}

void Stage3::BossInstance()
{
    pBoss_ = NewGO<Boss>(0, "boss");
    pBoss_->SetPos(nsStage3::nsBoss::InitPos);
}

void Stage3::TriggerInstace()
{
    // 設置座標。
    std::vector<Vector3> TriggerPosList =
    {
        nsStage3::nsTrigger::Pos1,
        nsStage3::nsTrigger::Pos2,
        nsStage3::nsTrigger::Pos3,
        nsStage3::nsTrigger::Pos4,
    };

    for (size_t i = 0; i < TriggerPosList.size(); i++)
    {
        pDimensionTrigger_ = NewGO<DimensionTrigger>(0, "dimensiontrigger");
        pDimensionTrigger_->SetTriggerPos(TriggerPosList[i]);
    }
}

void Stage3::WallActorInstace()
{
    // 設置座標。
    std::vector<Vector3> WallPosList =
    {
        nsStage3::nsWall::Pos1, 
        nsStage3::nsWall::Pos2,
        nsStage3::nsWall::Pos3,
        nsStage3::nsWall::Pos4,
    };

    // WallPosListの数だけループ。
    for (size_t i = 0; i < WallPosList.size(); i++)
    {
        // 透明壁生成。
        pWallActor_ = NewGO<WallActor>(0, "wallactor");

        // 座標設定。
        pWallActor_->SetWallPos(WallPosList[i]);

        // 回転を適応させる要素数も規則性はなし。
        if (i == 2 or i == 3)
        {
            Quaternion wallRot = Quaternion::Identity;
            wallRot.SetRotationDeg(Vector3::AxisY, 90.0f);
            pWallActor_->SetWallRot(wallRot);
        }
    }
}
