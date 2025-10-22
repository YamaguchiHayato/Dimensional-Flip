#pragma once
#include "Src/UI/UIBase.h"
#include "stdint.h"
#include <string>

class NumberUI : public UIBase
{
public:
	NumberUI() {};
	virtual ~NumberUI() {};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc)override;
	virtual const std::string InitUI(const std::string& UIname) override{
		return UIBase::InitUI(UIname);
	};

	/* タイマーの更新処理。*/
	void UpdateTimer();
	/* タイマーの開始。*/ 
	bool IsTimerStart() const
	{
		return isTimerStart_;
	}
	/* ファイルパス内のddsをまとめてInitする関数。 */
	void InitUINumber();

private:
	
	/* 数字表示用。*/
	SpriteRender UINumber_[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UINUmber_TensDigit[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UINUmber_OnesDigit[static_cast<int>(enUINumber::enNumber_Num)];

/* ローカル変数。*/
private:
	/* タイムUI描画フラグ。*/
	bool isTimeUIDrawing_ = true;						
	/* タイマー開始フラグ。*/
	bool isTimerStart_ = false;
	/* 時間切れフラグ。 */
	bool timeUpFlag_ = false;		
	/* 制限時間。*/ 
	float timer_ = 90.0f;
	/* 色の変化。*/
	float colorChange_ = 0.0f;													
	/* 10の位。*/
	int tenPlace_ = 0;	
	/* 1の位。*/ 
	int onePlace_ = 0;								

};

