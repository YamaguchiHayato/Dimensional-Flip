#include "stdafx.h"
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/SceneManager.h"

#include "Src/Title.h"
#include "Fade.h"



TitleScene::~TitleScene()
{
    // Titleオブジェクトの削除。
    DeleteGO(pTitle_);
}

bool TitleScene::Start()
{

    // Titleオブジェクトの生成。
    pTitle_ = NewGO<Title>(0, "title");
    if (pTitle_ == nullptr)
    {
        // Titleの生成に失敗するとシーン開始も失敗させる。
        return false;
    }

    return true;
}

void TitleScene::Update()
{

    // タイトルクラスでフラグが切り替わったら。
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        // InGameシーンへ遷移。
        SceneManager::GetInstance()->ChangeScene(SceneID::sInGame);
    }
}

