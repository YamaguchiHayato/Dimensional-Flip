#include "stdafx.h"
#include "Src/UI/UIBase.h"
#include "Src/UI/HPbarUI.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	const auto HEART_WIDTH = 199.0f;
	const auto HEART_HEIGHT = 187.0f;


	const auto FRAME_WIDTH = 252.0f;
	const auto FRAME_HEIGHT = 23.0f;

    const auto NUM_WIDHT = 300.0f;
    const auto NUM_HEIGHT = 300.0f;


	const Vector3 UI_SCALE{ 1.25f, 1.25f, 1.25f };
	const Vector3 HPBAR_SCALE(Vector3::One);
	const Vector3 FRAME_SCALE(Vector3::One);
	const Vector3 HEART_SCALE(Vector3::One);
    const Vector3 LIFE_SCALE(Vector3(0.5f, 0.5f, 0.5f));

	const Vector3 FillPOS { -700.0f, 450.0f,0.0f };
	const Vector3 FLAMEPOS{ -820.0f, 450.0f,0.0f };
	const Vector3 HEARTPOS{ -900.0f, 500.0f,0.0f };


    const Vector3 LEFTPOS{-850.0f, 550.0f, 0.0f};

    const auto HP_FONT_SCALE = 1.5f;
    const auto OUTLINE_OFFSET = 2.0f;

    const Vector3 FONT_OFFSET[5] =
    {
        { OUTLINE_OFFSET, -OUTLINE_OFFSET, 0.0f},
        { -OUTLINE_OFFSET, -OUTLINE_OFFSET, 0.0f},
        { OUTLINE_OFFSET, OUTLINE_OFFSET, 0.0f},
        { -OUTLINE_OFFSET, OUTLINE_OFFSET, 0.0f},
        { 0.0f, 00.0f, 0.0f}
    };
}


bool HPbarUI::Start()
{ 
	InitHPbar();
    pPlayer_ = FindGO<Player>("player");
	return true;
}


void HPbarUI::Update()
{
    // それぞれの座標を更新する。
	SetPositions();

    // それぞれのスケール値を更新する。
	SetScales();

    // それぞれの更新処理をまとめる関数。
	Updates();

    // HPの情報の取得とテキスト生成を行う。
    if (pPlayer_)
    {
        swprintf_s(hpText_, L"%d/%d", pPlayer_->GetHP(), pPlayer_->GetMaxHP());

        for (int i = 0; i < 5; ++i)
        {
            hpFont_[i].SetText(hpText_);
        }   
    }

    // HP色の更新処理。
    UpdateHPColor();
}


void HPbarUI::Render(RenderContext& rc)
{
	flame_.Draw(rc);
	heart_.Draw(rc);

    // ライフUIの描画。
    for (int i = 0; i < 5; ++i)
    {
        hpFont_[i].Draw(rc);
    }
}


void HPbarUI::InitHPbar()
{
	InitHPbar_Frame();

	InitHPbar_Heart();

    InitHPbar_Life();
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
    for (int i = 0; i < 5; ++i)
    {
        // 座標計算（ベース位置 + オフセット）
        Vector3 pos = LEFTPOS;
        pos.x += FONT_OFFSET[i].x;
        pos.y += FONT_OFFSET[i].y;

        hpFont_[i].SetPosition(pos);
        hpFont_[i].SetScale(HP_FONT_SCALE); // 大きく設定

        // 0-3は黒（縁取り）、4は白（本体）
        if (i < 4)
        {
            hpFont_[i].SetColor(Vector4::Black);
        }
        else
        {
            hpFont_[i].SetColor(Vector4::White);
        }
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
	flame_.SetPosition(FLAMEPOS);
	heart_.SetPosition(HEARTPOS);
}


void HPbarUI::SetScales()
{
	flame_.SetScale(FRAME_SCALE);
	heart_.SetScale(HEART_SCALE);
}


void HPbarUI::Updates()
{
	flame_.Update();
	heart_.Update();

}


void HPbarUI::UpdateHPColor()
{
    // デフォルトの色を指定。
    Vector4 color = Vector4::White;

    // HPの割合に応じて色を変更。
    if (!pPlayer_)
        return;

    if (pPlayer_->GetHP() <= 3)
        // 赤
        color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

    // ハートの色を変更する。
    heart_.SetMulColor(color);

    hpFont_[4].SetColor(color);
}
