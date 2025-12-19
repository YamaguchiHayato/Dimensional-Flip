#include "stdafx.h"
// ステージクラス。
#include "Src/Actor/Stage/MainUnit/Stage2.h"
#include "Src/Actor/Stage/IStage.h"
// キャラクタークラス。
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "Src/Actor/Character/Enemy/Thwomp.h"
// カメラクラス。
#include "Src/Core/CameraManager.h"
// データ統合クラス。
#include "Src/Core/Game.h"
// ギミック。
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"
#include "Src/Actor/Stage/Props/Box.h"
#include "Src/Camera/Dimensiontrigger.h"

namespace
{
        // 円運動用のdeg→rad変換
    const float DEG_TO_RAD = 3.1415926535f / 180.0f;

    const Vector3 STARTPOS(98.077f, 140.0f, 15.878f);
    const Vector3 SCALE(1.0f, 1.0f, 1.0f);
}

namespace GimmickParam
{
    // 回転トリック。
    namespace RotationFool
    {
        // リフト共通のY座標。
        const float COMMONBUTTOM_Y = 0.0f;

        // 座標設定。
        namespace Pos
        {
            // Y座標を上昇上限とする。
            const Vector3 Pos1(1250.0f, 200.0f, -60.0f);
            const Vector3 Pos2(1380.0f, 400.0f, -60.0f);
            const Vector3 Pos3(1480.0f, 600.0f, -60.0f);

            // 別ルート用。
            const Vector3 Pos4(1500.0f, 100.0f, -60.0f);
            const Vector3 Pos5(1500.0f, 100.0f, -60.0f);
            const Vector3 Pos6(1500.0f, 100.0f, -60.0f);
        }

        // リフトの移動速度リスト。
        const std::vector<float> SpeedList =
        {
            100.0f, // 1台目。
            150.0f, // 2台目。
            200.0f  // 3台目。
        };

    }


    // 箱。
    namespace Box
    {
        namespace Pos
        {
            const Vector3 Pos1(1690.0f, 610.0f, -60.0f);
        }
    }


    // 追従的。
    namespace TrackingEnemy
    {
        namespace Pos
        {
            const Vector3 Pos1(1000.0f, 300.0f, 15.878f);
        }
    }


    // トゥワンプ。
    namespace Thwomp
    {
        namespace Pos
        {
            const Vector3 Pos1(5400.0f, 1900.0f, -80.0f);
        }

        const float DISAPPEAR_Z = -300.0f;
        const float RESPAWN_DELAY = 3.0f;

        const Vector3 MOVE_DIR(-1.0f, -1.0f, 0.0f);    
        const float   MOVE_SPEED = 200.0f;

    }


    // Trigger。
    namespace Trigger
    {
        namespace Pos
        {
            // 開始地点。
            const Vector3 Pos2(3000.0f, 650.0f, -130.0f);
            // 終了地点。
            const Vector3 Pos1(5400.0f, 1700.0f, -130.0f);
            // テスト用初期地点。
            const Vector3 TestPos(200.0f, 0.0f, -70.0f);
        }
    }


    // ゴールの座標。
    namespace StarPos
    {
        namespace Pos
        {
            const Vector3 Pos1(5800.0f, 1800.0f, -80.0f);
        }
    }
} 

Stage2::~Stage2()
{
    for (auto* p : lDimensionTrigger_){ DeleteGO(p); }
    for (auto* p : lRotationFool_){ DeleteGO(p); }
    for (auto* p : lBox_){ DeleteGO(p); }
    for (auto* p : lStar_){ DeleteGO(p); }
    for (auto* p : lTrackingEnemy_){ DeleteGO(p);}
    for (auto* p : lThwomp_){ DeleteGO(p); }

    lDimensionTrigger_.clear();
    lRotationFool_.clear();
    lBox_.clear();
    lStar_.clear();
    lTrackingEnemy_.clear();
    lThwomp_.clear();
}

bool Stage2::Start()
{
    // ステージ2に使うオブジェクトの生成。
    // ステージ2モデル。
	const std::string stagePath = InitStage("Stage2/stage2");
	stageRender_.Init(stagePath.c_str());


    // 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;

    // 大きさ設定。
    stageRender_.SetScale(SCALE);
    stageRender_.Update();
	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());


    // ギミック生成。
    RotationFoolNewGO();
    // ボックス(足場)の生成。
    BoxInstance();
    // 敵生成。
    // TrackingInstance();
    // 回転敵の生成。
    ThwompInstance();
    // カメラトリガーの生成。
    DimensionTriggerInstance();
    //
    StarInstance();
    pPlayer = FindGO<Player>("player");
	return true;
}

void Stage2::Update()
{
    // モデルの更新処理。
	stageRender_.Update();
}

void Stage2::Render(RenderContext& rc)
{
	stageRender_.Draw(rc);
}

void Stage2::RotationFoolNewGO()
{
    // 設置座標。
    std::vector<Vector3> RotFoolPosList =
	{
        GimmickParam::RotationFool::Pos::Pos1,
        GimmickParam::RotationFool::Pos::Pos2,
        GimmickParam::RotationFool::Pos::Pos3
    };

    // 速度リスト。
    const auto& RotFoolSpeedList = GimmickParam::RotationFool::SpeedList;

    // 共通のY座標を定義する。
    const float CommonButtomY = GimmickParam::RotationFool::COMMONBUTTOM_Y;

    // リストの数が合っているかチェックする。
    if (RotFoolPosList.size() != RotFoolSpeedList.size()) return;

    // モデルの生成。
	for (size_t i = 0; i < RotFoolPosList.size(); i++)
	{
		auto rotFool = NewGO<RotationFool>(0, "rotationfool");

        // A. 上昇上限の座標を設定する。
        Vector3 topPos = RotFoolPosList[i];
        rotFool->SetPos(topPos);
        rotFool->SetTopPos(topPos);

        // B. 下降上限の座標を設置する。
        Vector3 bottomPos = topPos;
        bottomPos.y = CommonButtomY;
        rotFool->SetInitPos(bottomPos);

        // C. 速度を設定する。
        rotFool->SetMoveSpeed(RotFoolSpeedList[i]);

        // D. 大きさ。
        rotFool->SetScale(scale_);

        lRotationFool_.push_back(rotFool);
	}

}

void Stage2::BoxInstance()
{
    // 設置座標。
    std::vector<Vector3> BoxPosList =
	{
        GimmickParam::Box::Pos::Pos1,
    };

    for (size_t i = 0; i < BoxPosList.size(); i++)
    {
        pBox_ = NewGO<Box>(0, "Box");
        pBox_->SetPos(BoxPosList[i]);

        lBox_.push_back(pBox_);
    }
}

void Stage2::TrackingInstance()
{
    std::vector<Vector3> EnemySpawnList =
	{
        GimmickParam::TrackingEnemy::Pos::Pos1
    };

    for (size_t i = 0; i < EnemySpawnList.size(); i++)
    {
        pTrackingEnemy_ = NewGO<TrackingEnemy>(0, "TrackingEnemy");
        pTrackingEnemy_->SetPos(EnemySpawnList[i]);

        lTrackingEnemy_.push_back(pTrackingEnemy_);
    }

}

void Stage2::ThwompInstance()
{
    std::vector<Vector3> ThwompSpawnList =
    {
        GimmickParam::Thwomp::Pos::Pos1,
    };

    for (size_t i = 0; i < ThwompSpawnList.size(); i++)
    {
        spawnTimer_ += g_gameTime->GetFrameDeltaTime();

        // Thwompの生成。
        pThwomp_ = NewGO<Thwomp>(0, "Thwomp");
        // 座標。
        pThwomp_->InitPos(ThwompSpawnList[i]);
        // 削除ライン。
        pThwomp_->SetTriggerPos(GimmickParam::Trigger::Pos::Pos2);
    }
}

void Stage2::DimensionTriggerInstance()
{
    // 設置座標。
    std::vector<Vector3> TriggerPosList =
    {
        GimmickParam::Trigger::Pos::Pos2,
        GimmickParam::Trigger::Pos::Pos1,
        GimmickParam::Trigger::Pos::TestPos,
    };

    for (size_t i = 0; i < TriggerPosList.size(); i++)
    {
        pDimensionTrigger_ = NewGO<DimensionTrigger>(0,"dimensiontrigger");
        pDimensionTrigger_->SetTriggerPos(TriggerPosList[i]);

        lDimensionTrigger_.push_back(pDimensionTrigger_);
    }
}

void Stage2::StarInstance()
{
        // 設置座標。
    std::vector<Vector3> StarPosList =
    {
        GimmickParam::StarPos::Pos::Pos1,
    };

    for (size_t i = 0; i < StarPosList.size(); i++)
    {
        pStar_ = NewGO<Star>(0,"star");
        pStar_->SetStarPosition(StarPosList[i]);

        lStar_.push_back(pStar_);
    }

}
