#include "stdafx.h"

// タイトルクラス。
#include "Src/Scene/title/TitleView.h"

// ゲームシーンクラス。
#include "Src/Scene/TitleScene.h"

// 演出クラス。
#include "Src/Direction/Fade.h"

// ゲーム管理クラス。
#include "Src/Core/SceneManager.h"


TitleScene::~TitleScene()
{
    // Titleオブジェクトの削除。
    DeleteGO(pTitleView_);
}

bool TitleScene::Start()
{
    // Titleオブジェクトの生成。
    pTitleView_ = NewGO<TitleView>(0, "titleview");
    if (pTitleView_ == nullptr)
    {
        // Titleの生成に失敗するとシーン開始も失敗させる。
        return false;
    }

    // Fadeオブジェクトの取得。
    pFade_ = SceneManager::GetInstance()->GetFade();

    // フェード合うとフラグをリセット。
    isFadingOut = false;

    // シーン開始と同時にFadeStartを開始する。
    pFade_->StartFadeIn();

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
    pFade_->StartFadeOut();
}

