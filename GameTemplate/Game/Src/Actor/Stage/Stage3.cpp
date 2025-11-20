#include "stdafx.h"
#include "Src/Actor/Stage/Stage3.h"
#include "Src/Camera/Dimensiontrigger.h"

namespace nsStage3
{
    namespace Trigger
    {
        const Vector3 Pos1(1000.0f, 0.0f, 0.0f);
    }
}

bool Stage3::Start()
{
	const std::string stagePath = InitStage("Stage3/stage3");
	stageRender_.Init(stagePath.c_str());


    // 座標設定。
	stageRender_.SetPosition(stagePos_);
	initPos_ = stagePos_;

    // 大きさ設定。
    stageRender_.SetScale(Vector3::One);
    stageRender_.Update();
	stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());

    TriggerInstace();
    return true;
}

void Stage3::Update()
{
    // モデルの更新処理。
    stageRender_.Update();
}

void Stage3::Render(RenderContext& rc)
{
    stageRender_.Draw(rc);
}

void Stage3::TriggerInstace()
{
    std::vector<Vector3> TriggerList =
    {
        nsStage3::Trigger::Pos1,
    };

    for (size_t i = 0; i < TriggerList.size(); i++)
    {
        pDimensionTrigger_ = NewGO<DimensionTrigger>(0, "dimensiontrigger");
        pDimensionTrigger_->SetTriggerPos(TriggerList[i]);
    }
}
