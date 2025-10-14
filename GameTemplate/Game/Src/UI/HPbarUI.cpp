#include "stdafx.h"
#include "Src/UI/HPbarUI.h"

namespace
{
	const float UI_WIDTH = 300.0f;
	const float UI_HEIGHT = 300.0f;
	const Vector3 UI_SCALE{ 1.25f, 1.25f, 1.25f };
	const Vector3 HPBAR_SCALE{ 1.5f, 1.25f, 1.25f };
	const Vector3 HEARTSCALE{ 0.75f, 0.75f, 0.75f };

	const Vector3 FillPOS{ -700.0f, 450.0f,0.0f };
	const Vector3 GHOSTPOS{ -700.0f, 450.0f,0.0f };
	const Vector3 HEARTPOS{ -900.0f, 450.0f,0.0f };
}


bool HPbarUI::Start()
{ 
	/* HPbarとして必要な画像に必要な情報をそれぞれ初期化。*/
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
	/* Fillの描画。*/
	Fill_.Draw(rc);
	/* Ghostの描画。*/
//	Ghost_.Draw(rc);
	/* Heartの描画。*/
	Heart_.Draw(rc);
}

void HPbarUI::InitHPbar()
{
	/* HPバーのFill(HPbarとて残る部分)の初期化処理。*/
	InitHPbar_Fill();

	/* HPバーのGhost(Fillの残り値を示すために浸食する部分)の初期化処理。*/
	InitHPbar_Ghost();

	/* HPバーのHeartの初期化処理。*/
	InitHPbar_Heart();

}

void HPbarUI::InitHPbar_Fill()
{
	/* HPバー画像の初期化処理。*/
	const std::string HPbarUIPath = InitUI("progressBar/UI_HPbar_Fill");
	Fill_.Init(HPbarUIPath.c_str(), UI_WIDTH, UI_HEIGHT);

	Fill_.SetScale(HPBAR_SCALE);
	Fill_.SetPosition(FillPOS);
	Fill_.Update();
}

void HPbarUI::InitHPbar_Ghost()
{
	/* HPバー画像の初期化処理。*/
	const std::string HPbarUIPath = InitUI("progressBar/UI_HPbar_Ghost");
	Ghost_.Init(HPbarUIPath.c_str(), UI_WIDTH, UI_HEIGHT);

	Ghost_.SetScale(HPBAR_SCALE);
	Ghost_.SetPosition(GHOSTPOS);
	Ghost_.Update();

}

void HPbarUI::InitHPbar_Heart()
{
	/* HPバー画像の初期化処理。*/
	const std::string HPbarUIPath = InitUI("progressBar/UI_Heart");
	Heart_.Init(HPbarUIPath.c_str(), UI_WIDTH, UI_HEIGHT);

	Heart_.SetScale(HEARTSCALE);
	Heart_.SetPosition(HEARTPOS);
	Heart_.Update();
}

void HPbarUI::SetPositions()
{
	Fill_.SetPosition(FillPOS);
	Ghost_.SetPosition(GHOSTPOS);
	Heart_.SetPosition(HEARTPOS);
}

void HPbarUI::SetScales()
{
	Fill_.SetScale(HPBAR_SCALE);
	Ghost_.SetScale(HPBAR_SCALE);
	Heart_.SetScale(HEARTSCALE);
}

void HPbarUI::Updates()
{
	Fill_.Update();
	Ghost_.Update();
	Heart_.Update();
}