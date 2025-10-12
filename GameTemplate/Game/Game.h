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
class TimerUI;
class WallActor;
class NumberUI;
class Player;
class ScoreUI;
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
	 * @brief UIオブジェクトの生成。
	 */
	void UINewGO();

	/*
	 * @brief タイマーUIオブジェクトの生成。
	 */
	void TimerUINewGO();

	/*
	 * @brief 数字UIオブジェクトの生成。
	 */
	void NumberUINewGO();
	
	/*
	 * @brief スコアUIオブジェクトの生成。
	 */ 
	void ScoreUINewGO();

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
	TrackingEnemy* trackingEnemy_ = nullptr;
	Player* player_ = nullptr;

	// ステージ。
	Stage1* stage1_ = nullptr;
	Star* star_ = nullptr;
	WallActor* wallActor_ = nullptr;
	JumpPad* jumpPad_ = nullptr;

	/* UI。*/
	TimerUI* timerUI_ = nullptr;
	NumberUI* numberUI_ = nullptr;
	ScoreUI* scoreUI_ = nullptr;

	// その他。
	GameCamera* gameCamera_ = nullptr;
	SkyCube* skyCube_ = nullptr;
private:
	FontRender fontRender_; // デバッグ用。
	ModelRender modelRender_; // モデル。
	Vector3 Pos_;// 座標。
	Quaternion Rot_;
};

