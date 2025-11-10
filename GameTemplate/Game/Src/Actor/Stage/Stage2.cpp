#include "stdafx.h"
#include "Src/Actor/Stage/Stage2.h"
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"
#include "Game.h"
// ギミック。
#include "Src/Actor/Stage/Gimmick/RotationFool.h"

namespace
{
    const Vector3 STARTPOS(98.077f, 140.0f, 15.878f);
    const Vector3 SCALE(1.0f, 1.0f, 1.0f);
}

namespace Stage2GimmickPos
{
    // 回転トリック。
    namespace RotationFoolPosition
    {
        const Vector3 Pos1(1500.0f, 200.0f, 15.878f);
    }
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

    RotationFoolNewGO();
	return true;
}

void Stage2::Update()
{
    // モデルの更新処理。
	stageRender_.Update();

	// 当たり判定。
	//stagePhysics_.SetPosition(stagePos_);
}

void Stage2::Render(RenderContext& rc)
{
	stageRender_.Draw(rc);
}

void Stage2::RotationFoolNewGO()
{
    std::vector<Vector3> RotFoolPosList =
	{
		Stage2GimmickPos::RotationFoolPosition::Pos1
	};

	for (size_t i = 0; i < RotFoolPosList.size(); i++)
	{
		auto rotFool = NewGO<RotationFool>(0, "rotationfool");
		rotFool->SetRotFoolPosition(RotFoolPosList[i]);
        rotFool->SetScale(scale_);
	}

}

