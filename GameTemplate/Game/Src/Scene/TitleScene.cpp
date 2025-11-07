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

    // Fadeオブジェクトの取得。
    pFade_ = SceneManager::GetInstance()->GetFade();

    // フェード合うとフラグをリセット。
    isFadingOut = false;

    // シーン開始と同時にFadeStartを開始する。
    pFade_->FadeTransition(FadeState::FadeStart);

    return true;
}

void TitleScene::Update()
{
    if (pFade_ == nullptr) return;

    // まだフェードアウト中でない場合 (isFadingOut == false)
    // ＝ 入力待ち状態
    if (isFadingOut == false)
    {
        if (g_pad[0]->IsTrigger(enButtonA))
        {
        // フェードアウトを開始する。
        StartFadeOutToInGame();
        }
    }
    else
    {
        if (pFade_->IsFadeOutEnd())
        {
            SceneManager::GetInstance()->ChangeScene(SceneID::sInGame);
        }
    }
}

void TitleScene::StartFadeOutToInGame()
{
    // フラグを立ててUpdateでの入力を無効にする。
    isFadingOut = true;

    // FadeオブジェクトにFadeOutを指示。
    pFade_->FadeTransition(FadeState::FadeEnd);

}

