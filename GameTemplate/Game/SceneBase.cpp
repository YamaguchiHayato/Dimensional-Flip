#include "stdafx.h"
#include "SceneBase.h"
#include "SceneManager.h"

void Scene::Update()
{
	SceneManager::GetInstance()->Update();
};


