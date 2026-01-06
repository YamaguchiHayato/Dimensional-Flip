#include "stdafx.h"
#include "Src/UI/UIBase.h"
#include "Src/UI/HPbarUI.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	const auto HEART_WIDTH = 199.0f;
	const auto HEART_HEIGHT = 187.0f;

	const auto SLASH_WIDTH = 100.0f;
	const auto SLASH_HEIGHT = 100.0f;

	const auto FRAME_WIDTH = 252.0f;
	const auto FRAME_HEIGHT = 23.0f;

    const auto NUM_WIDHT = 300.0f;
    const auto NUM_HEIGHT = 300.0f;


	const Vector3 UI_SCALE{ 1.25f, 1.25f, 1.25f };
	const Vector3 SLASH_SCALE(Vector3(0.5f, 0.5f, 0.5f));
	const Vector3 HPBAR_SCALE(Vector3::One);
	const Vector3 FRAME_SCALE(Vector3::One);
	const Vector3 HEART_SCALE(Vector3(0.4f, 0.4f, 0.4f));
    const Vector3 LIFE_SCALE(Vector3(0.5f, 0.5f, 0.5f));

	const Vector3 FillPOS { -700.0f, 450.0f,0.0f };
	const Vector3 FLAMEPOS{ -820.0f, 450.0f,0.0f };
	const Vector3 HEARTPOS{ -900.0f, 480.0f,0.0f };

	const Vector3 SLASHPOS{ -750.0f, 480.0f,0.0f };

    const Vector3 LEFTPOS{-780.0f, 490.0f, 0.0f};
    const Vector3 LIGHTPOS{-720.0f, 490.0f, 0.0f};
}


bool HPbarUI::Start()
{ 
	InitHPbar();
    pPlayer_ = FindGO<Player>("player");
	return true;
}


void HPbarUI::Update()
{
	SetPositions();

	SetScales();

	Updates();

    for (int i = 0; i < static_cast<int>(enUINumber::enNumber_Num); i++)
    {
        life_Left[i].Update();
        life_light[i].Update();
    }
}


void HPbarUI::Render(RenderContext& rc)
{
	slash_.Draw(rc);
	flame_.Draw(rc);
	heart_.Draw(rc);
    if (pPlayer_)
    {
        int currentHP = pPlayer_->GetHP();

        // 配列外参照を防ぐクランプ
        if (currentHP < 0)
            currentHP = 0;
        if (currentHP >= static_cast<int>(enUINumber::enNumber_Num))
            currentHP = static_cast<int>(enUINumber::enNumber_Num) - 1;

        life_Left[currentHP].Draw(rc);

        // 右側：最大HP (Statusから取得するか、定数の9を使う)
        int maxHP = pPlayer_->GetMaxHP();
        if (maxHP >= static_cast<int>(enUINumber::enNumber_Num))
            maxHP = 9;

        // 配列名が life_light となっていますが右側の数字として扱います
        life_light[maxHP].Draw(rc);
    }
}


void HPbarUI::InitHPbar()
{
	InitHPbar_Slash();

	InitHPbar_Frame();

	InitHPbar_Heart();

    InitHPbar_Life();
}


void HPbarUI::InitHPbar_Slash()
{
	const std::string HPbarUIPath = InitUI("progressBar/Slash");
	slash_.Init(HPbarUIPath.c_str(), SLASH_WIDTH, SLASH_HEIGHT);

	slash_.SetScale(SLASH_SCALE);
	slash_.SetPosition(SLASHPOS);
	slash_.Update();
}


void HPbarUI::InitHPbar_Frame()
{
	const std::string HPbarUIPath = InitUI("progressBar/flame");
	flame_.Init(HPbarUIPath.c_str(), FRAME_WIDTH, FRAME_HEIGHT);

	flame_.SetScale(FRAME_SCALE);
	flame_.SetPosition(FLAMEPOS);
	flame_.Update();

}


void HPbarUI::InitHPbar_Life()
{
    for (uint8_t i = 0; i < static_cast<uint8_t>(enUINumber::enNumber_Num); i++)
    {
        // ライフUIの初期化。
        std::string filePath = "number/UI_" + std::to_string(i);
        std::string NumberUIPaht = InitUI(filePath);

        // ライフUI初期化。
        life_Left[i].Init(NumberUIPaht.c_str(), NUM_WIDHT, NUM_HEIGHT);
        life_light[i].Init(NumberUIPaht.c_str(), NUM_WIDHT, NUM_HEIGHT);

        // 座標。
        life_Left[i].SetPosition(LEFTPOS);
        life_light[i].SetPosition(LIGHTPOS);

        // スケール。
        life_Left[i].SetScale(LIFE_SCALE);
        life_light[i].SetScale(LIFE_SCALE);

      
    }
}


void HPbarUI::InitHPbar_Heart()
{
	const std::string HPbarUIPath = InitUI("progressBar/Heart");
	heart_.Init(HPbarUIPath.c_str(), HEART_WIDTH, HEART_HEIGHT);

	heart_.SetScale(HEART_SCALE);
	heart_.SetPosition(HEARTPOS);
	heart_.Update();
}


void HPbarUI::SetPositions()
{
	slash_.SetPosition(SLASHPOS);
	flame_.SetPosition(FLAMEPOS);
	heart_.SetPosition(HEARTPOS);
    life_Left->SetPosition(LEFTPOS);
    life_light->SetPosition(LIGHTPOS);
}


void HPbarUI::SetScales()
{
	slash_.SetScale(HPBAR_SCALE);
	flame_.SetScale(FRAME_SCALE);
	heart_.SetScale(HEART_SCALE);
    life_Left->SetScale(LIFE_SCALE);
    life_light->SetScale(LIFE_SCALE);
}


void HPbarUI::Updates()
{
	slash_.Update();
	flame_.Update();
	heart_.Update();
    life_Left->Update();
    life_light->Update();
}
