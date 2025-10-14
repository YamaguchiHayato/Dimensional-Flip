#pragma once
#include "Level3DRender/LevelRender.h"
/* Characterクラス。*/
class TrackingEnemy;
class Player;
/* UIクラス。*/
class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;
/* 透明な壁クラス。*/
class WallActor;
/* ステージクラス。*/
class Stage1;
/* ギミッククラス。*/
class Star;
class JumpPad;
/* カメラクラス。*/
class GameCamera;
/* ゲームクラス。*/
class Game : public IGameObject
{
public:
	Game() {};
	virtual ~Game();
	bool Start();
	void Update();
private:
	/* 
	 * @brief 追跡型敵オブジェクトの生成。
	 */
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
	 * @brief HPバーUIオブジェクトの生成。
	 */
	void HPbarUINewGO();

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
	HPbarUI* hpbarUI_ = nullptr;
	// その他。
	GameCamera* gameCamera_ = nullptr;
	SkyCube* skyCube_ = nullptr;
};

