#pragma once
#include "EnemyBase.h"

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
	Vector3 m_EnemyFP;
	Vector3 m_EnemyPosition;

	bool  m_isChasing = false;   // 追跡中フラグ
	float m_chaseSpeed = 4.0f;   // 追跡速度
	float m_triggerX = FLT_MAX;

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
	AnimationClip m_animationclip[enAnimationclip_num];
	CharacterController m_EnemyCC;
	SphereCollider m_sphereColl;
	Vector3 m_EnemyMS;
	Player* m_player = nullptr;

	Vector3 m_enemyDeathDistanceX = Vector3::Zero;//x軸の判定
	Vector3 m_enemyDeathDistanceY = Vector3::Zero;//y軸の判定
	ModelRender m_EnemyRender;
	Quaternion rotation;

private:
	int Enemystate = 0;
	int Enemyanimationstate = 0;

	bool m_touchPlayerFlag = false;//プレイヤーに触れたか？



private:
	/// <summary>
	/// 追跡処理。
	/// </summary>
	void Tracking();

	/// <summary>
    /// アニメーションメソッド。
    /// </summary>
	const std::string GetFullPath_EnemyAnimation(EnAnimationClip enemyAnimation, const std::string& animationName, bool flag);
	/// <summary>
	/// アニメーションをセット。
	/// </summary>
	void SetEnemyAnimation();

private:

	const char* ENEMY_ANIMATION = "Assets/animData/";// ファイルパス。
	const char* ANIMATION_FILE_EXTENSION = ".tka"; // 拡張子。

};







