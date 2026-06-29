#pragma once
#include "ResultData.h"


// リザルト画面の段階を管理する列挙型クラス。
enum class GameClearState : uint8_t
{
    FadeIn,     // 最初のフェード
    ShowLogo,   // RESULTロゴ表示
    CountTime,  // タイムのカウント演出
    CountScore, // スコアのカウント演出
    WaitInput,  // 入力待ち
};

class GameClear : public IGameObject
{
public:
	GameClear() = default;
	~GameClear() = default;


public:
	bool Start();
    void Update();
	void Render(RenderContext& rc);


private:
    // リザルトフレームUIを初期化する。
    void InitResultFrame();

    // 「GameClear」と表記。
    void InitGameClearFont();

    // タイム表示。
    void InitTimerFont();

    // スコア表示。
    void InitScoreFont();

    // 不要なUIの削除。
    void CleanUpUI();

    // 入力促進用のテキストを初期化。
    void InitInputPromptFont();

public:
    // データを受け取る処理。
    inline void SetUpResultData(const nsApp::nsStage::StageResultData& data)
    {
        resultData_ = data;
    }


// ゲッター。
public:
    // 演出が全て終わったか確認。
    inline bool IsFinished() const
    {
        return state_ == GameClearState::WaitInput;
    }


private:
    // リザルトフレーム画像の描画。
    SpriteRender resultFrameSprite_;
    // 「GameClear」と表記
    FontRender resultFont_;
    // タイム表示
    FontRender timerFont_;
    // スコア表示
    FontRender scoreFont_;
    // 「タイトルに戻る」と表記。
    FontRender inputPromptFont_;

    GameClearState state_ = GameClearState::ShowLogo;
    nsApp::nsStage:: StageResultData resultData_;

    float targetTime_ = 0.0f;
    float disPlayTime = 0.0f;
    float finishTimer_ = 0.0f;

    int targetScore_ = 0;
    int displayScore_ = 0;

    int timer_ = 0;

};

