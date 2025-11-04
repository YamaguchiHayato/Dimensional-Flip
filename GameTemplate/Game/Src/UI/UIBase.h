//
// @file    UIBase.h。
// @brief   UIの基底クラス。
// @details UIの派生クラスにこのクラスを継承させる。
// @dote    10/06 … クラス作成日。
//
#pragma once
class UIBase : public IGameObject
{

protected:
	UIBase() {};
	virtual ~UIBase() {};

    // 初期化処理関数。
	virtual bool Start() = 0 { return true; };
    // 更新処理関数。
	virtual void Update() = 0 {};
    // 描画処理関数。
	virtual void Render(RenderContext& rc) = 0 {};
    // UI用の画像パス初期化関数。
	virtual const std::string InitUI(const std::string& UIname) 
	{
		std::string UIpath = "Assets/stage/" + UIname + ".DDS";
		return UIpath;
	};
	

protected:
    // UI用のスプライトレンダー。
	SpriteRender UISprite_; 

protected:
    // UI用の数字列挙型。
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

