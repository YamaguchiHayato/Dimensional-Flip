#include "stdafx.h"

#include "GameplayScoreHudComponent.h"

namespace
{
    const Vector3 SCORE_LABEL_POS{450.0f, 520.0f, 0.0f};
    const Vector4 GOLD(1.0f, 0.85f, 0.0f, 1.0f);
    const int MAX_SCORE = 999999;
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void GameplayScoreHudComponent::SetNumberPosition(float x, float y)
        {
            /* SCORE 数値の表示位置を設定する。*/
            numPosX_ = x;
            numPosY_ = y;
        }


        void GameplayScoreHudComponent::OnBuild()
        {
            /* SCORE 数値の描画用フォントを初期化する。*/
            scoreFont_.SetScale(1.5f);
            scoreFont_.SetColor(GOLD);

            /* SCORE ラベルの描画用フォントを初期化する。*/
            scoreLabelFont_.SetText(L"SCORE");
            scoreLabelFont_.SetPosition(SCORE_LABEL_POS);
            scoreLabelFont_.SetScale(1.0f);
            scoreLabelFont_.SetColor(Vector4::White);
        }


        void GameplayScoreHudComponent::ApplyLogic(const ScoreDisplayLogic& logic)
        {
            /* スコアを取得し、キャッシュする。*/
            cachedScore_ = logic.GetScore();
            if (cachedScore_ > MAX_SCORE)
                cachedScore_ = MAX_SCORE;

            /* スコアを文字列に変換し、描画用フォントに設定する。*/
            swprintf_s(scoreText_, L"%06d", cachedScore_);
            scoreFont_.SetText(scoreText_);
            scoreFont_.SetPosition(Vector3(numPosX_, numPosY_, 0.0f));
        }


        void GameplayScoreHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            /* 親 Entity のワールド行列は使用しない。*/
            (void) ownerWorld;

            if (!isVisible_)
                return;

            /* SCORE ラベルと数値を描画する。*/
            scoreLabelFont_.Draw(rc);
            scoreFont_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
