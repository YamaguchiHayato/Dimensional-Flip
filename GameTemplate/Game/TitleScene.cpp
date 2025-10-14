#include "stdafx.h"
#include "TitleScene.h"
#include "Src/Title.h"
#include "SceneManager.h"

bool TitleScene::Start()
{
	title_ = NewGO<Title>(0, "title");
	return true;
}

void TitleScene::Update()
{
	if (title_->GetSceneTransitionFlag() == true)
	{
		SceneManager::GetInstance()->Update();
	}
}