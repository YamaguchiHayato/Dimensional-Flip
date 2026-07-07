#include "stdafx.h"

#include "Src/Presentation/UI/Components/UIScoreView.h"

namespace
{
    // 旧 ScoreUI と同じ定数
    const Vector3 SCORE_LABEL_POSITION{450.0f, 520.0f, 0.0f};
    const Vector4 GOLD(1.0f, 0.85f, 0.0f, 1.0f);
    const float FONT_SCALE = 1.5f;
    const float SCORE_SCALE = 1.0f;
    const int MAX_SCORE = 999999;
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        UIScoreView::UIScoreView() : posX_(0.0f), posY_(0.0f), digitSpacing_(20.0f), cachedScore_(0)
        {
            InitFonts();
        }


        void UIScoreView::InitFonts()
        {
            // 数値フォント（金色・大きめ）
            scoreFont_.SetScale(FONT_SCALE);
            scoreFont_.SetColor(GOLD);

            // "SCORE" ラベル（位置は固定・旧 UI 同様）
            scoreLabelFont_.SetText(L"SCORE");
            scoreLabelFont_.SetPosition(SCORE_LABEL_POSITION);
            scoreLabelFont_.SetScale(SCORE_SCALE);
            scoreLabelFont_.SetColor(Vector4::White);
        }


        void UIScoreView::SetPosition(float x, float y)
        {
            posX_ = x;
            posY_ = y;
            scoreFont_.SetPosition(Vector3(posX_, posY_, 0.0f));
        }


        void UIScoreView::SetDigitSpacing(float spacing)
        {
            digitSpacing_ = spacing;
        }


        void UIScoreView::ApplyLogic(const ScoreDisplayLogic& logic)
        {
            // ① GetScore() を ScoreDisplayLogic.h に追加してから使う
            cachedScore_ = logic.GetScore();
            if (cachedScore_ > MAX_SCORE)
                cachedScore_ = MAX_SCORE;

            // 旧 ScoreUI: swprintf_s(scoreText_, L"%06d", score_);
            swprintf_s(scoreText_, L"%06d", cachedScore_);
            scoreFont_.SetText(scoreText_);
            scoreFont_.SetPosition(Vector3(posX_, posY_, 0.0f));
        }


        void UIScoreView::Draw(RenderContext& rc)
        {
            if (!isVisible_)
                return;

            scoreLabelFont_.Draw(rc);
            scoreFont_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
