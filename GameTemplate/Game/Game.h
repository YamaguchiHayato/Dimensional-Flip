#pragma once
#include "Level3DRender/LevelRender.h"


enum class enStageState
{
	enState_Stage1,
	enState_Stage2,
	enState_Stage3,
	enState_Stage4,
	enState_Num,
};

class TrackingEnemy;
class WallActor;
class Player;
class Stage1;
class Star;
class JumpPad;
class GameCamera;
class GoalChecker;
class Game : public IGameObject
{
public:
	Game() {};
	virtual ~Game() {};
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
	 * @brief ジャンプパッドオブジェクトの生成。
	 */ 
	void JumpPadNewGO();

	/*
	 * @brief スターオブジェクトの生成。
	 */
	void StarNewGO();

	/*
	 * @brief ゴールフレームオブジェクトの生成。
	 */ 
	void GoalFrameNewGO();

	/*
 　　* @brief 視点回転が許可される領域をまとめて登録する。
     *
     * この関数では、ゲーム中でプレイヤーが視点回転を行えるエリアを
     * 初期化・設定する。各エリアは座標や境界情報として保持され、
     * カメラの回転可否判定に利用される。
     */
	void SetupViewRotationAreas();

	/*
	 * @brief スカイキューブの初期化。
	 */ 
	void InitSkyCube();
private:
	// モデル。
	TrackingEnemy* m_trackingEnemy = nullptr;
	Player* m_player = nullptr;

	// ステージ。
	Stage1* m_stage1 = nullptr;
	Star* m_star = nullptr;
	WallActor* m_wallActor = nullptr;
	JumpPad* m_jumpPad = nullptr;
	GoalChecker* m_goalChecker = nullptr;

	// その他。
	GameCamera* m_gameCamera = nullptr;
	SkyCube* m_skyCube = nullptr;
private:
	FontRender m_fontRender; // デバッグ用。
	ModelRender m_modelRender; // モデル。
	Vector3 m_pos;// 座標。
	Quaternion m_Rot;
};

