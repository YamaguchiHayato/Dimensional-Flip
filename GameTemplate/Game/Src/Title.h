#pragma once

/* タイトル画面でのGamePad操作。*/
enum class enGamePadSelect : uint8_t
{
	enSelect_AButton, /// Aボタン。
	enSelect_BButton, /// Bボタン。
	enSelect_XButton, /// Xボタン。
	enSelect_YButton, /// Yボタン。
	enSelect_LStick,  /// Lボタン。
	enSelect_RStick,  /// Rボタン。
	enSelect_DPad,    /// 十字キー。
	enSelect_Num,	  /// ゲームパッド数。
};

/* モード選択。。*/ 
enum class enTitleScene : uint8_t
{
	enTitle,     /// タイトル画面。
	enModeSelect,/// モード選択画面。
	enHowToPlay, /// 遊び方説明画面。
	enNum        /// シーン数。
};

/* タイトル画面でのシーン遷移。*/ 
enum class enTitleModeSelect : uint8_t
{
	enTitleTransition_Title,		///タイトル。
	enTitleTransition_ModeSelect,	///モード選択。
	enTitleTransition_HowToPlay,	///遊び方。
	enTitleTransition_Num			///遷移数。
};

class Title : public IGameObject
{
public:
	Title() {};
	~Title() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	bool GetSceneTransitionFlag()
	{
		return sceneTransitionFlag_; 
	}
	/* タイトル画面での入力操作。*/
	void TitleAction();	
private:
	/* タイトル画面の画像表示。*/ 
	SpriteRender titleRender_;

private:
	/* シーン遷移フラグ。*/
	bool sceneTransitionFlag_ = false;		
};

