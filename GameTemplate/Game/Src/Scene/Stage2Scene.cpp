#include "stdafx.h"
#include "Stage2Scene.h"
#include "Src/Scene/SceneManager.h"
#define SMGetIns SceneManager::GetInstance // シングルトンインスタンスを取得するマクロ定義

bool Stage2Scene::Start()
{
	SMGetIns()->SetRequest(SceneID::sStage2);
	/* ステージ2の生成。*/
	stage2_ = NewGO<Stage2>(0, "stage2");

	/* Playerクラスを取得。*/
	player_ = FindGO<Player>("player");
	if (player_ != nullptr)
	{
		/* プレイヤーをステージ2のスタートに移動。*/
		
	}
	return true;
}

void Stage2Scene::Update()
{

}