#include "stdafx.h"
#include "StageIcon.h"



void StageIcon::Init(const char* modelPath, StageID stageID)
{
    model_.Init(modelPath);
    model_.SetScale(Vector3::One * 0.25f);
    stageID_ = stageID;
}


void StageIcon::Update()
{
    model_.SetPosition(position_);
    model_.SetRotation(Quaternion::Identity);
    model_.Update();
}


void StageIcon::Render(RenderContext& rc)
{
    model_.Draw(rc);
}
