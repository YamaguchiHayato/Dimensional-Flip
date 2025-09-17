#pragma once
#include "Level3DRender/LevelRender.h"

class TrackingEnemy;
class Player;
class Stage1;
class GameCamera;
class Game : public IGameObject
{
public:
	Game() {};
	~Game() {};
	bool Start();
	void Update();
private:
	/// <summary>
	/// 追跡敵の生成。
	/// </summary>
	void EnemyNewGO_Tracking();

	/**
	 * @brief プレイヤーの座標を取得して表示。
	 */
	void FontRenderDebug();

	void Render(RenderContext);
private:
	// モデル。
	TrackingEnemy* m_trackingEnemy = nullptr;
	Player* m_player = nullptr;

	// ステージ。
	Stage1* m_stage1 = nullptr;

	// その他。
	GameCamera* m_gameCamera = nullptr;
	FontRender m_fontRender; // デバッグ用。
private:
	ModelRender m_modelRender; // モデル。
	Vector3 m_pos;// 座標。
	Quaternion m_Rot;
};

