#include "stdafx.h"
#include "Src/Core/Game.h"

// タイトルクラス。
#include "Src/Scene/title/TitleView.h"

// ゲームシーンクラス。
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/InGameScene.h"

// 演出クラス。
#include "Src/Direction/Fade.h"

// コアクラス。
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"

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

    app::core::SoundManager::GetInstance()->PlayBGM(GameSoundList_BGM_Title);

    // Fadeオブジェクトの取得。
    pFade_ = SceneManager::GetInstance()->GetFade();

    // フェード合うとフラグをリセット。
    isFadingOut = false;

    // シーン開始と同時にFadeInを開始する。
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
            //// フェードアウトを開始する。
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

