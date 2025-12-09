#include "stdafx.h"
#include "Src/UI/HPbarUI.h"

namespace
{
	const auto HEART_WIDTH = 456.0f;
	const auto HEART_HEIGHT = 458.0f;

	const auto SLASH_WIDTH = 300.0f;
	const auto SLASH_HEIGHT = 300.0f;

	const auto FRAME_WIDTH = 756.0f;
	const auto FRAME_HEIGHT = 152.0f;



	const Vector3 UI_SCALE{ 1.25f, 1.25f, 1.25f };
	const Vector3 HPBAR_SCALE(Vector3::One);
	const Vector3 FRAME_SCALE(Vector3::One);

	const Vector3 FillPOS{ -700.0f, 450.0f,0.0f };
	const Vector3 FLAMEPOS{ -500.0f, 450.0f,0.0f };
	const Vector3 HEARTPOS{ -900.0f, 450.0f,0.0f };
}


bool HPbarUI::Start()
{ 
	InitHPbar();
	return true;
}

void HPbarUI::Update()
{
	SetPositions();


	SetScales();


	Updates();
}

void HPbarUI::Render(RenderContext& rc)
{
//	Fill_.Draw(rc);
	Ghost_.Draw(rc);
	Heart_.Draw(rc);
}

void HPbarUI::InitHPbar()
{
	InitHPbar_Slash();

	InitHPbar_Frame();

	InitHPbar_Heart();
}


void HPbarUI::InitHPbar_Slash()
{
	const std::string HPbarUIPath = InitUI("progressBar/Slash");
	Fill_.Init(HPbarUIPath.c_str(), SLASH_WIDTH, SLASH_HEIGHT);

	Fill_.SetScale(HPBAR_SCALE);
	Fill_.SetPosition(FillPOS);
	Fill_.Update();
}


void HPbarUI::InitHPbar_Frame()
{
	const std::string HPbarUIPath = InitUI("progressBar/Frame");
	Ghost_.Init(HPbarUIPath.c_str(), FRAME_WIDTH, FRAME_HEIGHT);

	Ghost_.SetScale(FRAME_SCALE);
	Ghost_.SetPosition(FLAMEPOS);
	Ghost_.Update();

}


void HPbarUI::InitHPbar_Heart()
{
	const std::string HPbarUIPath = InitUI("progressBar/Heart");
	Heart_.Init(HPbarUIPath.c_str(), HEART_WIDTH, HEART_HEIGHT);

	Heart_.SetScale(HPBAR_SCALE);
	Heart_.SetPosition(FLAMEPOS);
	Heart_.Update();
}


void HPbarUI::SetPositions()
{
	Fill_.SetPosition(FillPOS);
	Ghost_.SetPosition(FLAMEPOS);
	Heart_.SetPosition(HEARTPOS);
}


void HPbarUI::SetScales()
{
	Fill_.SetScale(HPBAR_SCALE);
	Ghost_.SetScale(FRAME_SCALE);
	Heart_.SetScale(HPBAR_SCALE);
}

void HPbarUI::Updates()
{
	Fill_.Update();
	Ghost_.Update();
	Heart_.Update();
}
