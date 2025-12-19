#include "stdafx.h"
#include "Src/Actor/Stage/MainUnit/Stage1.h"
#include "Src/Actor/Character/Player.h"
#include "NormalEnemy.h"

#include "Src/Core/CameraManager.h"

// ギミック。
#include "Src/Actor/Stage/Gimmick/JumpPad.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/WallActor.h"
#include "Src/Actor/Stage/Gimmick/Wall.h"


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
        const Vector3 Pos1(10597.5, -31.7f, 210.0f);
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


    // 回転トリック。
    namespace RotationFool
    {
        // リフト共通のY座標。
        const float COMMONBUTTOM_Y = 0.0f;

        // 座標設定。
        namespace Pos
        {
            // Y座標を上昇上限とする。
            const Vector3 Pos1(2050.0f, 500.0f, -2200.0f);
            const Vector3 Pos2(2050.0f, 700.0f, -2400.0f);
            const Vector3 Pos3(2050.0f, 1000.0f, -2600.0f);

            // 別ルート用。
            const Vector3 Pos4(1500.0f, 100.0f, -60.0f);
            const Vector3 Pos5(1500.0f, 100.0f, -60.0f);
            const Vector3 Pos6(1500.0f, 100.0f, -60.0f);
        } 

        // リフトの移動速度リスト。
        const std::vector<float> SpeedList =
        {
            400.0f, // 1台目。
            450.0f, // 2台目。
            500.0f  // 3台目。
        };

    }


    // 壁。
    namespace Wall
    {
        const Vector3 Pos1 = Vector3(5400.0f, -50.0f, -40.0);
    }


    // 通常敵の出現位置。
    namespace NormalEnemyPos
    {
        const Vector3 Pos1 = Vector3(5400.0f, -50.0f, -20.0);

    }


    namespace BigEnemyPos
    {
        const Vector3 Pos1 = Vector3(6900.0f, -25.7f, -11.7f);
    }
}


struct  BigEnemySpawnParam
{
    void InitParam()
    {
    }
};


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

    for (auto* p : lRotationFool_)
    {
        DeleteGO(p);
    }

    for (auto* p : lWallInstance_)
    {
        DeleteGO(p);
    }


    lJumpPad_.clear();
    lStar_.clear();
    lWall_.clear();
    lDimensionTrigger_.clear();
    lRotationFool_.clear();
    lWallInstance_.clear();
}


bool Stage1::Start()
{

	const std::string stagePath = InitStage("Stage1/stage");
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
//    WallNewGO();

    // カメラトリガー生成。
    DimensionTriggerNewGO();

    // 回転トリック生成。
//    RotationFoolNewGO();

    // 壁インスタンス生成。
    WallCreateInstance();

    // ノーマルエネミー生成。
//    NormalEnemyCreateInstance();

	stageRender_.Update();

    pPlayer_ = FindGO<Player>("player");
	return true;
}


void Stage1::Update()
{
	stageRender_.Update();

	// 当たり判定。
	stagePhysics_.SetPosition(stagePos_);

    // Playerがカメラアクションをしたかチェックする関数。
    CheckCameraAction();
}


void Stage1::CheckCameraAction()
{
    Vector3 playerPos = pPlayer_->GetPlayerPos();
    // 判定範囲。
    auto checkRange = 100.0f * 100.0f;

    // 壁インスタンスとの距離を調べる。
    for (auto* wall : lWallInstance_)
    {
        Vector3 diff = wall->GetPos() - playerPos;

        // 距離の2乗で比較する。
        if (diff.LengthSq() < checkRange)
            // コリジョンを削除する。
            wall->DestroyCollision();
    }
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
       
		pWallActor_ = NewGO<WallActor>(0, "wall");

		if (i == 3) 
		{
			Quaternion wallRot;
			wallRot.SetRotationDegY(90.0f);
			pWallActor_->SetWallRot(wallRot);
		}

        pWallActor_->SetWallPos(WallPosList[i]);
        lWall_.push_back(pWallActor_);
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


void Stage1::RotationFoolNewGO()
{
    // ---------------------------------------------------
    // 1. 設置座標リスト
    // ---------------------------------------------------
    std::vector<Vector3> RotFoolPosList =
    {
        GimmickPos::RotationFool::Pos::Pos1,
        GimmickPos::RotationFool::Pos::Pos2,
        GimmickPos::RotationFool::Pos::Pos3,
    };

    // ---------------------------------------------------
    // 2. 速度リスト
    // ---------------------------------------------------
    const auto& RotFoolSpeedList = GimmickPos::RotationFool::SpeedList;

    // ---------------------------------------------------
    // 3. 回転の準備 (全て90度にする)
    // ---------------------------------------------------
    Quaternion rot90;

    // ---------------------------------------------------
    // 4. 生成ループ
    // ---------------------------------------------------

    // 共通のY座標
    const float CommonButtomY = GimmickPos::RotationFool::COMMONBUTTOM_Y;

    // 安全対策：座標と速度のリスト数が合っているかチェック
    if (RotFoolPosList.size() != RotFoolSpeedList.size())
        return;

    // モデルの生成
    for (size_t i = 0; i < RotFoolPosList.size(); i++)
    {
        auto rotFool = NewGO<RotationFool>(0, "rotationfool");

        // A. 座標の設定
        Vector3 topPos = RotFoolPosList[i];
        rotFool->SetPos(topPos);
        rotFool->SetTopPos(topPos);

        Vector3 bottomPos = topPos;
        bottomPos.y = CommonButtomY;
        rotFool->SetInitPos(bottomPos);

        // B. 速度の設定
        rotFool->SetMoveSpeed(RotFoolSpeedList[i]);

        // C. 回転の設定 【ここですべてに90度を適用】
        rotFool->SetDirection(rot90);

        // D. 大きさ
        rotFool->SetScale(scale_);

        // E. 回転角度の設定
        rot90.SetRotationDegY(90.0f);

        lRotationFool_.push_back(rotFool);
    }

    
}


void Stage1::WallCreateInstance()
{
    // --- (前半の壁生成コードはそのまま) ---
    std::vector<Vector3> WallPosList = {
        GimmickPos::Wall::Pos1,
    };

    for (size_t i = 0; i < WallPosList.size(); i++)
    {
        pWall_ = NewGO<app::stage::Wall>(0, "wall");
        pWall_->SetPos(WallPosList[i]);
        lWallInstance_.push_back(pWall_);
    }

}


void Stage1::NormalEnemyCreateInstance()
{
    // 密集生成箇所の生成位置。
    Vector3 startPos = GimmickPos::NormalEnemyPos::Pos1;

    // 敵同士の間隔
    float spacingX = 60.0f;
    float spacingZ = 60.0f;

    for (uint8_t x = 0; x < 3; x++)
    {
        for (uint8_t z = 0; z < 3; z++)
        {
            // 座標を計算
            Vector3 pos = startPos;
            pos.x += x * spacingX; // 横にずらす
            pos.z += z * spacingZ; // 奥にずらす

            // 少し空中に浮かせる場合（めり込み防止）
            pos.y += 10.0f;

            auto enemy = NewGO<app::enemy::NormalEnemy>(0, "enemy");
            enemy->SetPos(pos);
        }
    }

    // サイズを変えて生成。
    auto bigEnemy = NewGO<app::enemy::NormalEnemy>(0, "bigEnemy");

    Vector3 pos = GimmickPos::BigEnemyPos::Pos1;

    // 座標。
    bigEnemy->SetPos(pos);
    // 大きさ。
    bigEnemy->SetScale(Vector3(1.5f, 1.5f, 0.0f), 320.0f);
    // サイズが大きい敵は踏みつけて倒せないようにする。
    bigEnemy->SetIsStompable(false);
}
