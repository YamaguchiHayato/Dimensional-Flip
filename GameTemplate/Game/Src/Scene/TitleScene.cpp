#include "stdafx.h"
#include "Src/Core/Game.h"

// タイトルクラス。
#include "Src/Scene/title/TitleView.h"
#include "Src/UI/Select/TitleMenu.h"

// ゲームシーンクラス。
#include "Src/Scene/TitleScene.h"
#include "Src/Scene/InGameScene.h"

// 演出クラス。
#include "Src/Production/Fade.h"

// コアクラス。
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"

#include "system/system.h"


TitleScene::~TitleScene()
{
    // Titleオブジェクトの削除。
    DeleteGO(pTitleView_);

        // メニュー画面の削除。
        if (pTitleMenu_)
            DeleteGO(pTitleMenu_);
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

    // メニューUIの生成。
    pTitleMenu_ = NewGO<app::nsUI::TitleMenu>(1, "titleMenu");
    pTitleMenu_->Init();

    // タイトル画面BGMの再生。
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
    if (pFade_ == nullptr)
        return;

    if (isGameEndRequested_)
    {
        g_gameLoop_.isLoop_ = false;
        return;
    }

    // 入力待ち状態。
    if (isFadingOut == false)
    {
        // 入力待ち状態のときは、入力処理を行う。
        WaitInputAction();
    }


    else
    {
        if (pFade_->IsFadeOutEnd())
        {
            // ゲームスタート時の場合。
            if (selectType_ == TitleMenuType::GameStart)
                StartToInGame();

            // ゲーム終了時の場合。
            else if (selectType_ == TitleMenuType::GameEnd)
                RequestGameEnd();
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


void TitleScene::WaitInputAction()
{
    // 上入力を取得。
    isUp_ = g_pad[0]->IsTrigger(enButtonUp);
    // 下入力を取得。
    isDown_ = g_pad[0]->IsTrigger(enButtonDown);
    // 決定入力を取得。
    isDecide_ = g_pad[0]->IsTrigger(enButtonA);

    // 入力状態の更新。
    if (pTitleMenu_ && pTitleMenu_->Update(isUp_, isDown_, isDecide_))
    {
        // 決定された項目を取得する。
        selectType_ = pTitleMenu_->GetCurrentSelect();

        switch (selectType_)
        {
        case TitleMenuType::GameStart:
             nextSceneID_ = (int) SceneID::sWorldSelect;

             // フェードアウトを開始する。
             StartFadeOutToInGame();
             break;


        case TitleMenuType::Manual:
             // マニュアル画像を出すようにする。
             break;


        case TitleMenuType::GameEnd:
             StartFadeOutToInGame();
             break;
        }
    }
}


void TitleScene::StartToInGame()
{
    // フェードアウトがお終わり次第、選択したSceneにへ遷移。
    if (nextSceneID_ != -1)
        SceneManager::GetInstance()->ChangeScene((SceneID) nextSceneID_);
}


void TitleScene::RequestGameEnd()
{
    // フェードで画面が真っ暗になっている今、こっそり片付ける
    if (pTitleView_)
    {
        DeleteGO(pTitleView_);
        pTitleView_ = nullptr;
    }

    if (pTitleMenu_)
    {
        DeleteGO(pTitleMenu_);
        pTitleMenu_ = nullptr;
    }

    isGameEndRequested_ = true;
  
}
