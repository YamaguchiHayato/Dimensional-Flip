#include "stdafx.h"
#include "Src/UI/ScoreUI.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	const float UI_WIDTH = 300.0f;
	const float UI_HEIGHT = 300.0f;
	const Vector3 UI_POS{ 100.0f, 450.0f, 0.0f };

	const Vector4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);

    // 各桁のスコア画像の座標。
	const Vector3 HundredThousandDigit(730.0f, 490.0f, 0.0f);
	const Vector3 TenThousandDigit(770.0f, 490.0f, 0.0f);
	const Vector3 ThousandDigit(810.0f, 490.0f, 0.0f);
	const Vector3 HundredDigit(850.0f, 490.0f, 0.0f);
	const Vector3 TensDigit(890.0f, 490.0f, 0.0f);
	const Vector3 OnesDigit(930.0f, 490.0f, 0.0f);

	const Vector3 UI_SCALE(0.5f, 0.5f, 0.5f);

    // スコアの最大値。
	const int MAX_SCORE = 9999;

    // SCOREテキストの座標。
	const Vector3 ScoreTextPos{ 655.0f, 480.0f, 0.0f };

}


bool ScoreUI::Start()
{
    // UIスコアの初期化。
    InitUIScore();

    // プレイヤーの取得。
	pPlayer_ = FindGO<Player>("player");
	return true;
}

void ScoreUI::Update()
{
    // スコアの更新処理。
	UpdateScore();

    UIScoreText_.Update();
    UIScore_HundredThousandDigit[hundredThousandPlace_].Update();
    UIScore_TenThousandDigit[tenThousandPlace_].Update();
    UIScore_ThousandDigit[thousandPlace_].Update();
    UIScore_HundredDigit[hundredPlace_].Update();
    UIScore_TensDigit[tenPlace_].Update();
    UIScore_OnesDigit[onePlace_].Update();
}

void ScoreUI::Render(RenderContext& rc)
{
    // スコアテキストの描画。  
	UIScoreText_.Draw(rc);
    // 各桁のスコア描画。
    // 10万の桁の描画。
	UIScore_HundredThousandDigit[hundredThousandPlace_].Draw(rc);
    // 1万の桁の描画。
	UIScore_TenThousandDigit[tenThousandPlace_].Draw(rc);
    // 1000の桁の描画。
	UIScore_ThousandDigit[thousandPlace_].Draw(rc);
    // 100の桁の描画。
	UIScore_HundredDigit[hundredPlace_].Draw(rc);
    // 10の桁の描画。
	UIScore_TensDigit[tenPlace_].Draw(rc);
    // 1の桁の描画。
	UIScore_OnesDigit[onePlace_].Draw(rc);

}

void ScoreUI::InitUIScore()
{
    // スコアテキストのUI初期化。
	std::string filePath = InitUI("progressBar/UI_Score");
	UIScoreText_.Init(filePath.c_str(), UI_WIDTH, UI_HEIGHT);
	UIScoreText_.SetPosition(ScoreTextPos);
	UIScoreText_.SetScale(UI_SCALE);
	UIScoreText_.SetMulColor(WHITE);
	UIScoreText_.Update();


	for (int i = 0; i < static_cast<int>(enUINumber::enNumber_Num); i++)
	{
        // スコアの各桁のUI初期化。
		std::string filePath = "number/UI_" + std::to_string(i);
		std::string NumberUIPaht = InitUI(filePath);

        // 各桁のスコアUI初期化。
		UIScore_HundredThousandDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);
		UIScore_TenThousandDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);
		UIScore_ThousandDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);
		UIScore_HundredDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);
		UIScore_TensDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);
		UIScore_OnesDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);

        // 各桁のスコアUI位置設定。
		UIScore_HundredThousandDigit[i].SetPosition(HundredThousandDigit);
		UIScore_TenThousandDigit[i].SetPosition(TenThousandDigit);
		UIScore_ThousandDigit[i].SetPosition(ThousandDigit);
		UIScore_HundredDigit[i].SetPosition(HundredDigit);
		UIScore_TensDigit[i].SetPosition(TensDigit);
		UIScore_OnesDigit[i].SetPosition(OnesDigit);

        // 各桁のスコアUIスケール設定。
		UIScore_HundredThousandDigit[i].SetScale(UI_SCALE);
		UIScore_TenThousandDigit[i].SetScale(UI_SCALE);
		UIScore_ThousandDigit[i].SetScale(UI_SCALE);
		UIScore_HundredDigit[i].SetScale(UI_SCALE);
		UIScore_TensDigit[i].SetScale(UI_SCALE);
		UIScore_OnesDigit[i].SetScale(UI_SCALE);

        // 各桁のスコアUI色設定。
		UIScore_HundredThousandDigit[i].SetMulColor(WHITE);
		UIScore_TenThousandDigit[i].SetMulColor(WHITE);
		UIScore_ThousandDigit[i].SetMulColor(WHITE);
		UIScore_ThousandDigit[i].SetMulColor(WHITE);
		UIScore_TensDigit[i].SetMulColor(WHITE);
		UIScore_TensDigit[i].SetMulColor(WHITE);

        // 各桁のスコアUI更新。
		UIScore_HundredThousandDigit[i].Update();
		UIScore_TenThousandDigit[i].Update();
		UIScore_ThousandDigit[i].Update();
		UIScore_HundredDigit[i].Update();
		UIScore_TensDigit[i].Update();
		UIScore_OnesDigit[i].Update();
	}

}

void ScoreUI::UpdateScore()
{
    // ジャンプ成功時にスコアを加算。
	if (pPlayer_ != nullptr && pPlayer_->DoJumpCheck()) score_ += 100.0f;

	if (score_ > MAX_SCORE) score_ = MAX_SCORE;

    // 各桁のスコアを計算。
	int currentScore = static_cast<int>(score_);
	hundredThousandPlace_ = (currentScore / 100000) % 10;
	tenThousandPlace_ = (currentScore / 10000) % 10;
	thousandPlace_ = (currentScore / 1000) % 10;
	hundredPlace_ = (currentScore / 100) % 10;
	tenPlace_ = (currentScore / 10) % 10;
	onePlace_ = currentScore % 10;
}


