#include "stdafx.h"
#include "Src/Actor/Stage/Stage3.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Actor/Character/Player.h"
#include "Src/WallActor.h"
#include "Src/Actor/Stage/Gimmick/Star.h"

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

    namespace nsStar
    {
        const Vector3 Pos1(3500.0f, 39.2f, -4300.0f);
//        const Vector3 Pos1(100.0f, 39.2f, -10.0f);
    }
}

Stage3::~Stage3()
{
    // 透明壁の削除。
    for (WallActor* pWall : lWallActor_)
    {
        DeleteGO(pWall);
    }
    // リストをクリア(空に)。
    lWallActor_.clear();

    // カメラトリガーの削除。
    for (DimensionTrigger* pTrigger : lDimensionTrigger_)
    {
        DeleteGO(pTrigger);
    }
    // リストをクリア(空に)。
    lDimensionTrigger_.clear();

    // ゴールオブジェクト(Starクラス)の削除。
    for (Star* pStar : lStar_)
    {
        DeleteGO(pStar);
    }
    // リストをクリア(空に)。
    lStar_.clear();
}

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

    TriggerInstace();
    WallActorInstace();
//    StarInstance();

    pPlayer_ = FindGO<Player>("player");
    return true;
}

void Stage3::Update()
{

    if (isRotating_)
    {
        float speed = 30.0f * g_gameTime->GetFrameDeltaTime();
        // ヘッダーの変数名(targeteAngle_)に合わせています
        float diff = targeteAngle_ - currentAngle_;

        float step = 0.0f;

        if (std::abs(diff) < speed)
        {
            step = diff;
            isRotating_ = false; // 目標に到達したら終了
        }
        else
        {
            step = (diff > 0.0f) ? speed : -speed;
        }

        // 回転行列の作成。
        Quaternion rotStep = Quaternion::Identity;
        rotStep.SetRotationDeg(Vector3::AxisX, step);

        // ステージモデルの回転・移動
        Vector3 currentStagePos = stageRender_.GetPosition();
        Quaternion currentStageRot = stageRender_.GetRotation();

        // 位置の回転
        Vector3 relativePos = currentStagePos - rotPivot_;
        rotStep.Apply(relativePos);
        stageRender_.SetPosition(rotPivot_ + relativePos);
  
        stageRender_.SetScale(Vector3::One);
  
        // 向きの回転。
        currentStageRot.Multiply(rotStep);
        stageRender_.SetRotation(currentStageRot);
        stageRender_.Update();
  
        stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());
  
         // Bossも一緒に回転させる。
        if (pBoss_)
        {
            Vector3 bossPos = pBoss_->GetPos();
            Vector3 relBoss = bossPos - rotPivot_;
            rotStep.Apply(relBoss);
            pBoss_->SetPos(rotPivot_ + relBoss);

            // ボスの向きも回転させる。
            Quaternion bossRot = pBoss_->GetRot();
            bossRot.Multiply(rotStep);
            pBoss_->SetRot(bossRot);
        }
    }

    stageRender_.SetScale(Vector3::One);
    // モデルの更新処理。
    stageRender_.Update();
}

void Stage3::RequestRotStage(float angle, const Vector3& pivot)
{
    if (isRotating_)
        return;

    isRotating_ = true;
    targeteAngle_ = angle;
    currentAngle_ = 0.0f;
    rotPivot_ = pivot;
}

void Stage3::Render(RenderContext& rc)
{
    stageRender_.Draw(rc);
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
        // 古い値を使わないようにループ内で宣言。
        DimensionTrigger* pDimensionTrigger = nullptr;
        pDimensionTrigger= NewGO<DimensionTrigger>(0, "dimensiontrigger");
        pDimensionTrigger->SetTriggerPos(TriggerPosList[i]);
        pDimensionTrigger->SetStage(this);

        // リストに追加して管理する。
        lDimensionTrigger_.push_back(pDimensionTrigger);
    }

}


void Stage3::WallActorInstace()
{
    WallActor* pWallActor = nullptr;

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
        pWallActor = NewGO<WallActor>(0, "wallactor");

        // 座標設定。
        pWallActor->SetWallPos(WallPosList[i]);

        // 回転を適応させる要素数も規則性はなし。
        if (i == 2 || i == 3)
        {
            Quaternion wallRot = Quaternion::Identity;
            wallRot.SetRotationDeg(Vector3::AxisY, 90.0f);
            pWallActor->SetWallRot(wallRot);
        }

        // リストに追加して管理する。
        lWallActor_.push_back(pWallActor);

    }
}


void Stage3::StarInstance()
{
    // 設置座標
    std::vector<Vector3> starPosList =
    {
        // 座標。
        nsStage3::nsStar::Pos1,
    };

    // ループ処理。
    for (uint8_t i = 0; i < starPosList.size(); i++)
    {
        pStar_ = NewGO<Star>(0, "star");
        pStar_->SetStarPosition(starPosList[i]);

        // リストに追加して管理する。
        lStar_.push_back(pStar_);
    }
}
