//
// @file HPbarUI.h
// @dote 10/14 … クラス追加日。
//

#pragma once
#include "Src/UI/UIBase.h"
class Player;

class HPbarUI : public UIBase
{
public:
	HPbarUI() {};
	virtual ~HPbarUI() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
	const std::string InitUI(const std::string& UIname) override {
		return UIBase::InitUI(UIname);
	};


private:
	void InitHPbar();
	void InitHPbar_Heart();
	void InitHPbar_Slash();
	void InitHPbar_Frame();
    void InitHPbar_Life();

    // それぞれのSetPositionをまとめる関数。
	void SetPositions();
    // それぞれのSetScaleをまとめる関数。
	void SetScales();
    // HPバーの更新処理。
	void Updates();

private:
    Player* pPlayer_ = nullptr;

private:
    // HPバー関連変数。
    SpriteRender heart_;  // HPバーのハート(装飾)部分。
    SpriteRender life_;   // HPバーのライフ。
    SpriteRender slash_;  
    SpriteRender flame_;  // HPバーのGhost部分。

	SpriteRender life_Left[static_cast<uint8_t>(enUINumber::enNumber_Num)];
    SpriteRender life_light[static_cast<uint8_t>(enUINumber::enNumber_Num)];
    
};

