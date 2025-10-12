#include "stdafx.h"
#include "NumberUI.h"

namespace
{
	const float UI_WIDTH = 300.0f;
	const float UI_HEIGHT = 300.0f;
	const Vector3 UI_POS{ 100.0f, 450.0f, 0.0f };

	const Vector4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);


	const Vector3 TensDigit(-45.0f, 450.0f, 0.0f); 
	const Vector3 OnesDigit(45.0f, 450.0f, 0.0f);   
	const Vector3 UI_SCALE(1.0f, 1.0f, 1.0f );
}

bool NumberUI::Start()
{
	InitUINumber();
	isTimerStart_ = true;

	return true;
}

void NumberUI::Update()
{
	UpdateTimer();
}

void NumberUI::Render(RenderContext& rc)
{
	/* 10の桁の描画。*/
	UINUmber_TensDigit[tenPlace_].Draw(rc);
	/* 1の桁の描画。*/ 
	UINUmber_OnesDigit[onePlace_].Draw(rc);
}

void NumberUI::InitUINumber()
{
	for (int i = 0; i <static_cast<int>( enUINumber::enNumber_Num); i++)
	{
		/* UI画像のInit*/
		std::string filePath = "number/UI_" + std::to_string(i);
		std::string NumberUIPaht = InitUI(filePath);

		UINUmber_TensDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);
		UINUmber_OnesDigit[i].Init(NumberUIPaht.c_str(), UI_WIDTH, UI_HEIGHT);

		/* 座標の設定。*/
		UINUmber_TensDigit[i].SetPosition(TensDigit);
		UINUmber_OnesDigit[i].SetPosition(OnesDigit);

		/* 大きさの設定。*/
		UINUmber_TensDigit[i].SetScale(UI_SCALE);
		UINUmber_OnesDigit[i].SetScale(UI_SCALE);

		/* 乗算カラーの設定。*/
		UINUmber_TensDigit[i].SetMulColor(WHITE);
		UINUmber_TensDigit[i].SetMulColor(WHITE);

		/* 更新処理。*/
		UINUmber_TensDigit[i].Update();
		UINUmber_OnesDigit[i].Update();
	}

}

void NumberUI::UpdateTimer()
{
	/* タイマーの処理。*/
	if (timer_ > 0.0f)
	{
		timer_ -= g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		timer_ = 0.0f;
	}

	int remainingSeconds = static_cast<int>(timer_); 
	tenPlace_ = (remainingSeconds / 10) % 10;   
	onePlace_ = remainingSeconds % 10;        
}

