#pragma once
#include "Src/UI/UIBase.h"
#include "Src/Actor/Character/PlayerForward.h"


class ScoreUI : public UIBase
{
public:
	ScoreUI()  = default;
	virtual ~ScoreUI() = default;


public:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


public:
    // スコアの更新処理。
	void UpdateScore();

    // スコアをドラムロール式に加算する処理。
    void UpdateScoreScroll();

    // スコアのリセット処理。
    inline void ResetScore()
    {
        score_ = 0.0f;
    }


private:
    // UIの初期化処理。
	void InitUIScore();


public:
    // Scoreを取得する。
    inline float GetScore() const
    {
        return static_cast<int>(score_);
    }

    inline static ScoreUI* GetInstance()
    {
        return instance_;
    }

private:
    static ScoreUI* instance_;


private:
	Player* pPlayer_ = nullptr;


private:
    FontRender scoreFont_;
    FontRender scoreFontUIRender_;


private:
    // スコア関連変数。
	float score_ = 0.0f;

    // 文字を表示する用。
    wchar_t scoreText_[64];

};

