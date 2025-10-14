#include "stdafx.h"
#include "Stage1Scene.h"
#include "Src/Title.h"
#include "SceneManager.h"
#define SMGetIns SceneManager::GetInstance // シングルトンインスタンスを取得するマクロ定義

// ステージ1シーン::初期化処理。
bool Stage1Scene::Start()
{
	// ステージ1の初期化処理を行う。
	stage1_ = NewGO<Stage1>(0, "Stage1"); // ステージ1の初期化処理を行う。

	// 

	//インスタンス
	//タイトルシーンのタイトルクラスを取得する。
	title_ = FindGO<Title>("Title");
	if (title_ != nullptr)
	{
		title_->SetGameLooadFlag(true); 
	}
	return true;
}

