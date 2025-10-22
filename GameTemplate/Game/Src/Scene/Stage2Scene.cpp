#include "stdafx.h"
#include "Src/Scene/Stage2Scene.h"
#include "Src/Scene/SceneManager.h"
#define SMGetIns SceneManager::GetInstance // シングルトンインスタンスを取得するマクロ定義

bool Stage2Scene::Start()
{
	SMGetIns()->SetRequest(SceneID::sStage2);
	stage2_ = NewGO<Stage2>(0, "stage2");

	player_ = FindGO<Player>("player");
	if (player_ != nullptr)
	{
		
	}
	return true;
}
