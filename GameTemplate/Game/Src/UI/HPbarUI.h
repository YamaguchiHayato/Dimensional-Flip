/*
 * @file HPbarUI.h
 * @dote 10/14 … クラス追加日。
 */

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
	/* HPバーの画像類をInitする処理。*/
	/* それぞれのInit関数をInitする処理。*/
	void InitHPbar();
	/* HP画像をInit。*/
	void InitHPbar_Heart();
	/* HPバーのFillをInit。*/
	void InitHPbar_Fill();
	/* HPバーのGhostをInit。*/
	void InitHPbar_Ghost();


	/* それぞれのSetPositionをまとめる関数。*/
	void SetPositions();
	/* それぞれのSetScaleをまとめる関数。*/ 
	void SetScales();
	/* それぞれのUpdateをまとめる関数。*/
	void Updates();

private:
	SpriteRender Heart_;
	SpriteRender HPText_;
	SpriteRender Fill_;
	SpriteRender Ghost_;

};

