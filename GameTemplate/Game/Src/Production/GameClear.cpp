#include "stdafx.h"

#include "Src/Scene/title/TitleView.h"
#include "Src/Scene/TitleScene.h"

#include "Src/Production/GameClear.h"
#include "Src/Production/StageClear.h"

#include "Src/UI/Tutorial/TutorialCompleteUI.h"
#include "Src/UI/Tutorial/TutorialSequencer.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"

bool GameClear::Start()
{
    // 初期ステートとタイマーを初期化する。
    state_ = GameClearState::FadeIn;
    timer_ = 0.0f;

    // フレーム画像の初期化。
    InitResultFrame();

    // Gameclearテキストの初期化。
    InitGameClearFont();

    // タイム表示の初期化。
    InitTimerFont();

    // スコア表示の初期化。
    InitScoreFont();

    // 不要なUIを削除する。
    CleanUpUI();
	return true;
}


void GameClear::Update()
{
    switch (state_)
    {
    case GameClearState::FadeIn:
        if (++timer_ > 60)
        {
            state_ = GameClearState::CountTime; 
            timer_ = 0;
        }
        break;

    case GameClearState::CountTime:
        if (disPlayTime < resultData_.clearTime_)
        {
            disPlayTime += resultData_.clearTime_ / 30.0f;
        }
        else
        {
            disPlayTime = resultData_.clearTime_;
            state_ = GameClearState::CountScore;
        }
        break;

    case GameClearState::CountScore:
        if (displayScore_ < resultData_.baseScore_)
        {
            displayScore_ += max(1, resultData_.baseScore_ / 30);
        }
        else
        {
            displayScore_ = resultData_.baseScore_;
            state_ = GameClearState::WaitInput;
        }
        break;
    }

    wchar_t timeStr[64];
    swprintf_s(timeStr, L"TIME: %d", static_cast<int>(disPlayTime));
    timerFont_.SetText(timeStr);

    wchar_t scoreStr[64];
    swprintf_s(scoreStr, L"SCORE: %d", static_cast<int>(displayScore_));
    scoreFont_.SetText(scoreStr);
}


void GameClear::Render(RenderContext& rc)
{
    // リザルト画面のフレームの描画。
    resultFrameSprite_.Draw(rc);

    // 「Game Clear」の描画。
    resultFont_.Draw(rc);

    // タイムとスコアの描画。
    timerFont_.Draw(rc);
    scoreFont_.Draw(rc);
}


void GameClear::InitResultFrame()
{
    resultFrameSprite_.Init("Assets/UI/resultFrame.DDS", 1500, 800);
    resultFrameSprite_.SetPosition({0.0f, 0.0f, 0.0f});
    resultFrameSprite_.SetScale(Vector3::One * 1.5f);
}


void GameClear::InitGameClearFont()
{
    resultFont_.SetText(L"Game Clear");
    resultFont_.SetPosition({-500.0f, 500.0f, 0.0f});
    resultFont_.SetScale(3.0f);
    resultFont_.SetColor(Vector4::White);
}


void GameClear::InitTimerFont()
{
    timerFont_.SetPosition(Vector3(-280.0f, -50.0f, 0.0f));
    timerFont_.SetScale(1.5f);
}


void GameClear::InitScoreFont()
{
    scoreFont_.SetPosition(Vector3(-280.0f, -200.0f, 0.0f));
    scoreFont_.SetScale(1.5f);
}


void GameClear::CleanUpUI()
{
    auto* pTutorialMsg = FindGO<app::nsUI::TutorialMessageUI>("TutorialMessageUI");
    if (pTutorialMsg)
        DeleteGO(pTutorialMsg);

    auto* pOrangeLogo = FindGO<StageClear>("stage_clear_logo");
    if (pOrangeLogo)
        DeleteGO(pOrangeLogo);

    auto* pTutorialComp = FindGO<app::nsUI::TutorialCompleteUI>("TutorialCompleteUI");
    if (pTutorialComp)
        DeleteGO(pTutorialComp);

    auto* pTutorialSeq = FindGO<app::nsUI::TutorialSequencer>("TutorialSequencer");
    if (pTutorialSeq)
        DeleteGO(pTutorialSeq);
}
