#include "stdafx.h"
#include "Fade.h"
#include "Src/Scene/FadeScene.h"
#include "Src/Scene/SceneManager.h"
bool FadeScene::Start()
{
    // シーン開始と同時にFadeEndを開始する。
    pFade_ = SceneManager::GetInstance()->GetFade();

    if (pFade_ == nullptr)
    {
        SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
        return false;
    }

    pFade_->FadeTransition(FadeState::FadeStart);
    return true;
}

void FadeScene::Update()
{
    if (pFade_ == nullptr) return;

    if (pFade_->IsFadeInEnd())
    {
        // 完了したら TitleScene に遷移します
        SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
    }
}
