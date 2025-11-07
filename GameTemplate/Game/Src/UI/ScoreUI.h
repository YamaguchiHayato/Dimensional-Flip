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
    // スコアの更新処理。
	void UpdateScore();
    // スコアのリセット処理。
    inline void ResetScore()
    {
        score_ = 0.0f;
    }

private:
    // UIの初期化処理。
	void InitUIScore();
private:
    // プレイヤーのポインタ。
	Player* pPlayer_ = nullptr;
private:
    // スコア表示用スプライト。
	SpriteRender UIScoreText_;
    // スコア数字列挙型。
    // 各桁の数字を管理するための列挙型。
	SpriteRender UIScore_HundredThousandDigit[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UIScore_TenThousandDigit[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UIScore_ThousandDigit[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UIScore_HundredDigit[static_cast<int>(enUINumber::enNumber_Num)];		
	SpriteRender UIScore_TensDigit[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UIScore_OnesDigit[static_cast<int>(enUINumber::enNumber_Num)];

private:
    // スコア関連変数。
	float score_ = 0.0f;
    // 100000の位。
	uint8_t hundredThousandPlace_ = 0;
    // 10000の位。
	uint8_t tenThousandPlace_ = 0;
    // 1000の位。
	uint8_t thousandPlace_ = 0;
    // 100の位。
	uint8_t hundredPlace_ = 0;
    // 10の位。
	uint8_t tenPlace_ = 0;
    // 1の位。
	uint8_t onePlace_ = 0;

};

