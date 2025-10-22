#pragma once
/// <summary>
/// ステージクリア
/// </summary>
class Player;
class Fade;
class GameCamera;
class GameTimer;
class Game;
class StageClear : public IGameObject
{
public:
	bool Start();	//開始処理。
	void Update();	//更新処理。
	void Render(RenderContext& rc);	 //描画処理。

	void SetIsClear(bool clear)
	{
		IsClear = clear;
	}

	const bool& GetIsClear()const
	{
		return IsClear;
	}


	bool IsClear = false;

private:
	/// <summary>
	/// ステージクリアスプライト用のイージングを設定。
	/// </summary>
	void SetStageClearSpriteEasing();
	/// <summary>
	/// ステージクリアスプライト用のイージングの更新処理。
	/// </summary>
	void UpdateStageClearSpriteEasing();
	/// <summary>
	/// ローディング処理
	/// </summary>
	void LoadingProcess();

	SpriteRender stageClearUI_;										//ステージクリアUI。
	Vector3 position_ = Vector3(-1100.0f, 275.0f, 0.0f);				//位置。
	Vector3 m_beforeEasingPosition = Vector3::Zero;						//イージング前の位置。
	Vector3 m_afterEasingPosition = Vector3::Zero;						//イージング後の位置。
	float m_easingTime = 0.0f;										    //割合。
	bool m_stageClearDirectionFinishFlag = false;						//ステージクリア演出終了フラグ。
	Player* m_player = nullptr;											//プレイヤー用のインスタンス。
	Fade* m_fade = nullptr;												//フェード用のインスタンス。
	GameCamera* m_gameCamera = nullptr;									//ゲームカメラ用のインスタンス。
	GameTimer* m_gameTimer = nullptr;									//ゲームタイマー用のインスタンス。
	Game* m_game = nullptr;												//ゲーム用のインスタンス。
	bool m_loadingStartFlag = false;									//ローディング開始フラグ
	bool m_loadingFinishFlag = false;									//ローディング終了フラグ
	bool m_stage2TransitionFlag = false;								//ステージ2遷移フラグ
};

