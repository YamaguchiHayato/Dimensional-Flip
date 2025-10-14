#include "stdafx.h"
#include "SceneManager.h"
#include "SceneBase.h"
#include "TitleScene.h"
#define SMGetIns SceneManager::GetInstance
SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::Update()
{
	/* IDが無効ではない場合、シーン遷移を行う。*/
	if (requestID_ != SceneID::sInvalid)
	{
		ChangeScene();
	}
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
		delete scene_;

		switch (requestID_)
		{
		case SceneID::sTitle:
			scene_ = new TitleScene();
			requestID_ = SceneID::sInvalid;
			break;
		case SceneID::sStage1:
			break;
		case SceneID::sStage2:
			break;
		case SceneID::sGameOver:
			break;
		case SceneID::sGameClear:
			break;
		case SceneID::sResult:
			break;
		case SceneID::sInvalid:
			break;
		default:
			break;
		}
	}

	/* シーンが作成されるとStart()を呼び出す。*/
	if (scene_)
	{
		/* シーンを初期化する。*/
		scene_->Start();
		/* リクエストされたシーンを無効化する。*/
		requestID_ = SceneID::sInvalid;
	}
}	