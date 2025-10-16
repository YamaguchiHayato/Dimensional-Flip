#pragma once
#include "Src/Actor/Character/Enemy/EnemyBase.h"

class Player;
/// <summary>
/// 派生クラス : 追跡敵。
/// </summary>
class TrackingEnemy :  public EnemyBase
{
// 外部参照用 関数。
public:
	TrackingEnemy() {};
	~TrackingEnemy() {};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void Move();
	void Rotation();
	void EnemyAnimation();

// 外部参照用 変数。
public:
	Vector3 enemyFP_;
	Vector3 enemyPosition_;

	bool  isChasing_ = false;   // 追跡中フラグ
	float chaseSpeed_ = 4.0f;   // 追跡速度
	float triggerX_ = FLT_MAX;

public:
	enum class State
	{
		Patrol,
		Chase
	};

	enum EnAnimationClip
	{
		enAnimationclip_idle,
		enAnimationclip_walk,
		enAnimationclip_death,
		enAnimationclip_num
	};
private:
	AnimationClip animationclip_[enAnimationclip_num];
	CharacterController enemyCC_;
	SphereCollider sphereColl_;
	Vector3 enemyMS_;
	Player* player_ = nullptr;

	Vector3 enemyDeathDistanceX_ = Vector3::Zero;//x軸の判定
	Vector3 enemyDeathDistanceY_ = Vector3::Zero;//y軸の判定
	ModelRender enemyRender_;
	Quaternion rotation_;

private:
	int enemystate_ = 0;
	int enemyanimationstate_ = 0;

	bool touchPlayerFlag_ = false;//プレイヤーに触れたか？



private:
	/* 追跡処理。*/
	void Tracking();

	/* 
	 * @brief アニメーションのフルパスを取得する関数。
	 */ 
	const std::string GetFullPath_EnemyAnimation(EnAnimationClip enemyAnimation, const std::string& animationName, bool flag);

	/* 
	 * @brief アニメーションを格納する関数。
	 */ 
	void SetEnemyAnimation();

private:

	const char* ENEMY_ANIMATION = "Assets/animData/";// ファイルパス。
	const char* ANIMATION_FILE_EXTENSION = ".tka"; // 拡張子。

};







