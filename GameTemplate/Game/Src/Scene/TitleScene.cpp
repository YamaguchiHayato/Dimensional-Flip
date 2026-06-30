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

    // マニュアルUIの削除。
    if (pManualUI_)
        DeleteGO(pManualUI_);
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

    pTitleMenu_->Open();

    // マニュアルUIの生成。
    pManualUI_ = NewGO<app::nsUI::ManualUI>(1, "ManualUI");
    pManualUI_->Init();

    // タイトル画面BGMの再生。
    app::core::SoundManager::GetInstance()->ReleaseAllSounds();
    app::core::SoundManager::GetInstance()->PlayBGM(GameSoundList_BGM_Title);

    // Fadeオブジェクトの取得。
    pFade_ = SceneManager::GetInstance()->GetFade();

    // 自動ロード画面表示を無効にする。
    SceneManager::GetInstance()->SetEnableAutoLoading(false);

    // フェード合うとフラグをリセット。
    isFadingOut = false;

    // 初期の状態をセットする。
    titleState_ = TitleState::Normal;

    // シーン開始と同時にFadeInを開始する。
    pFade_->StartFadeIn();

    return true;
}


void TitleScene::Update()
{
    if (pFade_ == nullptr)
        return;

    // ゲーム終了がリクエストされている場合は、ゲームループを終了させる。
    if (isGameEndRequested_)
    {
        g_gameLoop_.isLoop_ = false;
        return;
    }

    UpdateTitleState();
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
        selectType_ = pTitleMenu_->GetCurrentSelect();

        switch (selectType_)
        {
        case TitleMenuType::GameStart:
             nextSceneID_ = (int) SceneID::sWorldSelect;

             // 本編開始時はロード画面が必要なのでONにする。
             SceneManager::GetInstance()->SetEnableAutoLoading(true);

             // フェードアウト開始
             pFade_->StartFadeOut();
             titleState_ = TitleState::GameStartFade;
             break;

        case TitleMenuType::Manual:
             pFade_->StartFadeOut();
             app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button);

             titleState_ = TitleState::FadingToManual;
             break;

        case TitleMenuType::GameEnd:
             pFade_->StartFadeOut();
             titleState_ = TitleState::GameEndFade;
             break;
        }
    }
}


void TitleScene::UpdateTitleState()
{
    switch (titleState_)
    {
    case TitleState::Normal:
         UpdateNormalState();
         break;

    case TitleState::FadingToManual:
         UpdateFadingToManualState();
         break;

    case TitleState::ManualOpen:
         UpdateManualOpenState();
         break;

    case TitleState::FadingToMenu:
         UpdateFadingToMenuState();
         break;

    case TitleState::GameStartFade:
         UpdateGameStartFadeState();
         break;

    case TitleState::GameEndFade:
         UpdateGameEndFadeState();
         break;

    default:
        break;
    }
}


void TitleScene::UpdateNormalState()
{
    if (!pFade_->IsFadeOutEnd())
        WaitInputAction();
}


void TitleScene::UpdateFadingToManualState()
{
    // 画面が完全に暗くなったら切り替える
    if (pFade_->IsFadeOutEnd())
    {
        if (pTitleMenu_)
            pTitleMenu_->Close();

        if (pManualUI_)
            pManualUI_->Open();

        // ロゴを消す。
        if (pTitleView_)
        {
            pTitleView_->SetShowLogo(false);
            pTitleView_->SetManualMode(true);

            pTitleView_->SetShowInformation(false);
        }

        pFade_->StartFadeIn();
        titleState_ = TitleState::ManualOpen;
    }
}


void TitleScene::UpdateManualOpenState()
{
    // ManualUI →　Title画面に戻る処理。
    // フェードイン完了後、Bボタン入力をまつ。
    if (g_pad[0]->IsTrigger(enButtonB))
    {
        // マニュアルUIを閉じる。
        pFade_->StartFadeOut();

        // キャンセルSEを鳴らす。
        app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SelectScreen_Cancel);

        // タイトルメニューを開く。
        titleState_ = TitleState::FadingToMenu;
    }
}


void TitleScene::UpdateFadingToMenuState()
{
    // 画面が完全に暗くなったら元に戻す。
    if (pFade_->IsFadeOutEnd())
    {
        if (pManualUI_)
            pManualUI_->Close();

        if (pTitleMenu_)
            pTitleMenu_->Open();

        // ロゴを再表示させる
        if (pTitleView_)
        {
            pTitleView_->SetShowLogo(true);
            pTitleView_->SetManualMode(false);

            pTitleView_->SetShowInformation(false);
        }

        pFade_->StartFadeIn();
        titleState_ = TitleState::Normal;
    }
}


void TitleScene::UpdateGameStartFadeState()
{
    // フェードアウトが終わったらInGameSceneに遷移する。
    if (pFade_->IsFadeOutEnd())
        if (nextSceneID_ != -1)
            SceneManager::GetInstance()->ChangeScene((SceneID) nextSceneID_);
}


void TitleScene::UpdateGameEndFadeState()
{
    if (pFade_->IsFadeOutEnd())
    {
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

        if (pManualUI_)
        {
            DeleteGO(pManualUI_);
            pManualUI_ = nullptr;
        }

        isGameEndRequested_ = true;
    }
}
