#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/UI/ScoreUI.h"

namespace
{
    const Vector3 SCORE_NUM_POSITION{625.0f, 550.0f, 0.0f};
    const Vector3 SCORE_FONT_POSITION{450.0f, 520.0f, 0.0f};

    const Vector4 GOLD(1.0f, 0.85f, 0.0f, 1.0f);

    const float FONT_SCALE = 1.5;
    const float SCORE_SCALE = 1.0;

    // スコアの最大値（6桁なら999,999）
    const int MAX_SCORE = 999999;
}


ScoreUI* ScoreUI::instance_ = nullptr;


bool ScoreUI::Start()
{
    instance_ = this;
    InitUIScore();
    pPlayer_ = FindGO<Player>("player");
    return true;
}


void ScoreUI::Update()
{
    // スコアのドラムロール計算
    UpdateScoreScroll();

    // スコアの計算処理。
    UpdateScore();
}


void ScoreUI::Render(RenderContext& rc)
{
    scoreFont_.Draw(rc);

    scoreFontUIRender_.Draw(rc);
}


void ScoreUI::InitUIScore()
{
    // FontRenderの基本設定
    scoreFont_.SetPosition(SCORE_NUM_POSITION);
    scoreFont_.SetScale(FONT_SCALE);
    scoreFont_.SetColor(GOLD);

    // ScoreFontUI
    scoreFontUIRender_.SetText(L"SCORE");
    scoreFontUIRender_.SetPosition(SCORE_FONT_POSITION);
    scoreFontUIRender_.SetScale(SCORE_SCALE);
    scoreFontUIRender_.SetColor(Vector4::White);

}


void ScoreUI::UpdateScore()
{
    if (score_ > MAX_SCORE)
        score_ = (float) MAX_SCORE;

    // スコア本体の位置。
    swprintf_s(scoreText_, L"%06d", static_cast<int>(score_));
    scoreFont_.SetText(scoreText_);

    // スコアの文字UIの位置。
    swprintf_s(scoreText_, L"%06d SCORE", static_cast<int>(score_));
}


void ScoreUI::UpdateScoreScroll()
{
    if (!pPlayer_)
        return;

    auto targetScore = static_cast<float>(pPlayer_->GetScore());
    if (targetScore > MAX_SCORE)
        targetScore = (float) MAX_SCORE;

    if (score_ < targetScore)
    {
        auto diff = targetScore - score_;
        auto addScore = diff * 0.2f; // 20%ずつ加算
        if (addScore < 1.0f)
            addScore = 1.0f;
        score_ += addScore;
        if (score_ > targetScore)
            score_ = targetScore;
    }
    else if (score_ > targetScore)
    {
        score_ = targetScore;
    }
}
