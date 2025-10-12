/*
 * @file    UIBase.h。
 * @brief   UIの基底クラス。
 * @details UIの派生クラスにこのクラスを継承させる。
 * @dote    10/06 … クラス作成日。
 */
#pragma once
class UIBase : public IGameObject
{

protected:
	UIBase() {};
	virtual ~UIBase() {};

	/* 継承用関数の初期化処理。*/
	/* 初期化処理関数。*/
	virtual bool Start() = 0 { return true; };
	/* 更新処理関数。*/	
	virtual void Update() = 0 {};
	/* 描画処理関数。*/
	virtual void Render(RenderContext& rc) = 0 {};
	/* 画像のInit関数。*/
	virtual const std::string InitUI(const std::string& UIname) 
	{
		std::string UIpath = "Assets/UI/" + UIname + ".DDS";
		return UIpath;
	};
	

protected:
	/* 画像表示用。*/
	SpriteRender UISprite_; 
	/* UIを表示させる座標。*/
	Vector2 UIPos_ = Vector2::Zero;

protected:
	/* UIの数字情報を管理する列挙型。*/
	static enum class enUINumber : uint8_t
	{
		enNumber_Zero, /// 0。
		enNumber_One,  /// 1。
		enNumber_Two,  /// 2。
		enNumber_Three,/// 3。
		enNumber_Four, /// 4。
		enNumber_Five, /// 5。
		enNumber_Six,  /// 6。
		enNumber_Seven,/// 7。
		enNumber_Eight,/// 8。
		enNumber_Nine, /// 9。 
		enNumber_Num,  /// 数。
	};


};

