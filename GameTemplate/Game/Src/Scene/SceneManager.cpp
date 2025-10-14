#include "stdafx.h"
#include "Src/Scene/SceneManager.h"
#include "Src/Scene/Stage1Scene.h"
#include "Src/Actor/Stage/Stage2.h"
#include "Src/Scene/Scene.h"
#include "Src/Scene/TitleScene.h"
#define SMGetIns SceneManager::GetInstance
SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::Update()
{
	/* IDが無効ではない場合、シーン遷移を行う。*/
	if (requestID_ != SceneID::sInvalid)
	{
		ChangeScene();
	}

	/* シーンが存在する場合、更新処理を行う。*/
	if (scene_) scene_->Update();
}

void SceneManager::Render(RenderContext& rc)
{
	if (scene_) scene_->Render(rc);
}
void SceneManager::ChangeScene()
{
	/* 既存シーンの解放。*/
	if (scene_)
	{
		delete scene_;
		scene_ = nullptr;
	}

	/* IDが無効でなければ。*/
	if (requestID_ != SceneID::sInvalid)
	{
		switch (requestID_)
		{
		case SceneID::sTitle:
			scene_ = NewGO<TitleScene>(0,"titlescene");
			requestID_ = SceneID::sInvalid;
			break;
		case SceneID::sStage1:
			scene_ = NewGO<Stage1Scene>(0, "stage1scene");
			requestID_ = SceneID::sInvalid;
			break;
		case SceneID::sStage2:
			break;
		case SceneID::sGameOver:
			break;
		case SceneID::sGameClear:
			break;
		case SceneID::sResult:
			break;
		default:
			break;
		}
	}

	/* シーンが作成されるとStart()を呼び出す。*/
	if (scene_) scene_->Start();
	/* リクエストされたシーンを無効化する。*/
	requestID_ = SceneID::sInvalid;

}	