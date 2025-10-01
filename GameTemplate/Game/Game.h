#pragma once
#include "Level3DRender/LevelRender.h"

class TrackingEnemy;
class WallActor;
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

	/*
	 * @brief 透明壁オブジェクトの生成。
	 */
	void WallNewGO();
	/*
 　　* @brief 視点回転が許可される領域をまとめて登録する。
     *
     * この関数では、ゲーム中でプレイヤーが視点回転を行えるエリアを
     * 初期化・設定する。各エリアは座標や境界情報として保持され、
     * カメラの回転可否判定に利用される。
     */
	void SetupViewRotationAreas();

private:
	// モデル。
	TrackingEnemy* m_trackingEnemy = nullptr;
	Player* m_player = nullptr;

	// ステージ。
	Stage1* m_stage1 = nullptr;
	WallActor* m_wallActor = nullptr;

	// その他。
	GameCamera* m_gameCamera = nullptr;
	FontRender m_fontRender; // デバッグ用。
private:
	ModelRender m_modelRender; // モデル。
	Vector3 m_pos;// 座標。
	Quaternion m_Rot;
};

