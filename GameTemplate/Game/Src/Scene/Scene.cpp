#include "stdafx.h"
#include "Scene.h"
#include "SceneManager.h"

void SceneBase::Update()
{
	SceneManager::GetInstance()->Update();
}