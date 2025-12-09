//
// @file HPbarUI.h
// @dote 10/14 … クラス追加日。
//

#pragma once
#include "Src/UI/UIBase.h"
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
    // HPバーUIの初期化処理。
	void InitHPbar();
    // HPバーのTextをInit。
	void InitHPbar_Heart();
    // HPバーのFillをInit。
	void InitHPbar_Slash();
    // HPバーのGhostをInit。
	void InitHPbar_Frame();


    // それぞれのSetPositionをまとめる関数。
	void SetPositions();
    // それぞれのSetScaleをまとめる関数。
	void SetScales();
    // HPバーの更新処理。
	void Updates();

private:
    // HPバー関連変数。
    SpriteRender Heart_; // HPバーのテキスト部分。
    SpriteRender HPText_;// HPバーのテキスト。
    SpriteRender Fill_;  // HPバーのFill部分。
    SpriteRender Ghost_; // HPバーのGhost部分。

};

