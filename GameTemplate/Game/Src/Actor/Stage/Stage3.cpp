#include "stdafx.h"
#include "stage3.h"
#include "Src/Actor/Stage/IStage.h"

bool Stage3::Start()
{
    std::string stagePath = InitStage("Stage3/stage3");
    stageRender_.Init(stagePath.c_str());

    render_.SetPosition(stagePos_);
    render_.SetScale(Vector3::One);

    stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());
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
