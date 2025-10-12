#pragma once
#include "Src/UI/UIBase.h"
class Player;
class ScoreUI : public UIBase
{
public:
	ScoreUI() {};
	virtual ~ScoreUI() {};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	const std::string InitUI(const std::string& UIname) override
	{
		return UIBase::InitUI(UIname);
	}
	/* スコアの更新処理。*/
	void UpdateScore();
private:
	/* スコアの画像をまとめてInitする処理。*/
	void InitUIScore();

private:
	/* プレイヤーのポインタ。*/
	Player* player_ = nullptr;
private:
	/* SCOREの画像表示用スプライト。*/
	SpriteRender UIScoreText_;
	/* スコアの桁表示用スプライト。*/
	/* 100000の位。*/
	SpriteRender UIScore_HundredThousandDigit[static_cast<int>(enUINumber::enNumber_Num)];
	/* 10000の位。*/
	SpriteRender UIScore_TenThousandDigit[static_cast<int>(enUINumber::enNumber_Num)];
	/* 1000の位。*/
	SpriteRender UIScore_ThousandDigit[static_cast<int>(enUINumber::enNumber_Num)];
	/* 100の位。*/
	SpriteRender UIScore_HundredDigit[static_cast<int>(enUINumber::enNumber_Num)];		
	/* 10の位。*/
	SpriteRender UIScore_TensDigit[static_cast<int>(enUINumber::enNumber_Num)];
	/* 1の位。*/
	SpriteRender UIScore_OnesDigit[static_cast<int>(enUINumber::enNumber_Num)];

private:
	/* スコア。*/
	float score_ = 0.0f;
	/* 100000の位。*/
	int hundredThousandPlace_ = 0;
	/* 10000の位。*/
	int tenThousandPlace_ = 0;
	/* 1000の位。*/
	int thousandPlace_ = 0;
	/* 100の位。*/
	int hundredPlace_ = 0;
	/* 10の位。*/
	int tenPlace_ = 0;
	/* 1の位。*/
	int onePlace_ = 0;

};

