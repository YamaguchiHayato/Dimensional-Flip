#include "stdafx.h"
#include "ChaseEnemy.h"
#include "Player.h"

namespace
{
	// キャラコンの定数。
	const auto RADIUS = 25.0f;
	const auto HEIGHT = 30.0f;

    // モデルの大きさ。
	const Vector3 SCALE(5.0f, 5.0f, 5.0f);

	// モデルの設定パラメータ。
	const auto GLAVITY = 3.0f;
}

/// <summary>
/// アニメーションのパスを取得。
/// </summary>
const::std::string ChaseEnemy::FetchFullPath_EnemyAnimation(EnEnemyAnimation enemyAnimation, const std::string& animationName, bool flag)
{
	std::string Animation = ENEMY_ANIMATION + animationName + ANIMATION_FILE_EXTENSION;

	// アニメーションのロード。
	m_animationclip[enemyAnimation].Load(Animation.c_str());
	m_animationclip[enemyAnimation].SetLoopFlag(flag);

	return Animation;
}

/// <summary>
/// モデルのパスを取得。
/// </summary>
//const std::string ChaseEnemy::FetchFullPath_EnemyModel(const std::string& modelName)
//{
//	std::string Model = "Assets/modelData/TestEnemy/" + modelName + ".tkm";
//	m_ChaseEnemyRender.Init(Model.c_str());
//
//	return Model;
//}

/// <summary>
/// 初期化処理。
/// </summary>
bool ChaseEnemy::Start()
{
	// アニメーションのセット。
	SetEnemyAnimation(); 

	// 追従敵のモデルの初期化。
	m_ChaseEnemyRender.Init("Assets/modelData/Enemy/Skeleton/Skeleton.tkm", m_animationclip, enemyAnimation_idle, enModelUpAxisY, true);

	// モデルの大きさ。
	m_ChaseEnemyRender.SetScale(SCALE);
	
	// キャラコンの設定。
	m_ChaseEnemyCC.Init(RADIUS, HEIGHT, m_ChaseEnemyPos);

	// 座標の設定。
	m_ChaseEnemyRender.SetPosition(m_ChaseEnemyPos);

	// 回転の設定。
	m_Rot.SetRotationDegX(270.0f);
	m_ChaseEnemyRender.SetRotation(m_Rot);

	// モデルの更新処理。
	m_ChaseEnemyRender.Update();

	// Playerの探索。
	m_player = FindGO<Player>("player"); // プレイヤーの取得。
	return true;
}

/// <summary>
/// 更新処理。
/// </summary>
void ChaseEnemy::Update()
{
	// ヌルチェック。
	if (m_player != nullptr)
	{
		return;
	}

	// 動作処理。
	Move();

	// 追従処理。
//	Chase();                     

	// 回転処理。
	Rotation();                  

	// アニメーション制御処理。
	EnemyAnimation();           

	// 追従敵の更新処理。
	m_ChaseEnemyRender.Update(); 


}

/// <summary>
/// 移動制御処理。
/// </summary>
void ChaseEnemy::Move()
{
	//// ステートが0の時(パトロール中)。
	//if (enemyState == 0) m_ChaseEnemySpeed.x = -2.0f;

	//// 追従時。
	//else if (enemyState == 1) m_ChaseEnemySpeed.x = 2.0f;

	//// 移動速度。
	//m_ChaseEnemyPos = m_ChaseEnemyCC.Execute(m_ChaseEnemySpeed, 1.0f);

	//// モデルにかかる重力の設定。
	//SetGravity();


	// 歩いているとき。
	if (enemyState == 0) {
		m_ChaseEnemySpeed.x = -2.0f;
	}

	// 死亡時。
	else if (enemyState == 1) {
		m_ChaseEnemySpeed.x = 2.0f;
	}
	if (m_ChaseEnemyPos.x >= firstposition.x + 200.0f)
	{
		enemyState = 0;
	}
	else if (m_ChaseEnemyPos.x <= firstposition.x - 200.0f)
	{
		enemyState = 1;
	}
	if (enemyAnimationState == 1) {
		m_ChaseEnemySpeed.x = 0;
		if (!m_ChaseEnemyRender.IsPlayingAnimation())
		{
			DeleteGO(this);
		}
	}
	m_ChaseEnemyPos = m_ChaseEnemyCC.Execute(m_ChaseEnemySpeed, 1.0f);
	float glavity = 3.0f;
	if (m_ChaseEnemyCC.IsOnGround()) {
		//重力をなくす
		m_ChaseEnemySpeed.y = 0.0f;
	}
	m_ChaseEnemySpeed.y -= glavity;

}

/// <summary>
/// 回転処理。
/// </summary>
void ChaseEnemy::Rotation()
{
	if (fabsf(m_ChaseEnemySpeed.x) >= 0.001f || fabsf(m_ChaseEnemySpeed.z) >= 0.001f) {
		m_Rot.SetRotationYFromDirectionXZ(m_ChaseEnemySpeed);
		m_ChaseEnemyRender.SetRotation(m_Rot);
	}

}

/// <summary>
/// 描画処理。
/// </summary>
void ChaseEnemy::Render(RenderContext& rc)
{
	m_ChaseEnemyRender.Draw(rc);
}

/// <summary>
/// 追従処理。
/// </summary>
void ChaseEnemy::Chase()
{
	Vector3 ChaseDistance; // 感知距離。
	ChaseDistance = m_player->m_position - m_ChaseEnemyPos; // プレイヤーと追従敵の距離を計算。
    

	// 距離が50.0f以内なら追従。
	if(ChaseDistance.Length() < 50.0f)
	{
		// ベクトルの正規化。
		ChaseDistance.Normalize();

		// 求めた座標を利用して敵の座標を動かす。
		m_ChaseEnemyPos += ChaseDistance * 2.0f;
	}

	//// 50.0f以上なら追従しない。
	//else
	//{

	//}

}

/// <summary>
/// モデルの重力に関係するコードをまとめておく処理。
/// </summary>
void ChaseEnemy::SetGravity()
{
	auto GLAVITY = 3.0f;
	if (m_ChaseEnemyCC.IsOnGround())
	{
		// 重力を無くす。
		m_ChaseEnemySpeed.y = 0.0f;
	}

	m_ChaseEnemySpeed.y -= GLAVITY;
}

/// <summary>
/// アニメーション制御。
/// 将来的には親クラスから継承を行う。
/// </summary>
void ChaseEnemy::EnemyAnimation()
{
	// アニメーション制御
	switch (enemyAnimationState)
	{
	// 待機モーション。
	case EnEnemyAnimation::enemyAnimation_idle:
		m_ChaseEnemyRender.PlayAnimation(enemyAnimation_idle, 0.1f);
		break;

	// 走りモーション。
	case EnEnemyAnimation::enemyAnimation_run:
		m_ChaseEnemyRender.PlayAnimation(enemyAnimation_run, 0.1f);
		break;

	// 撃破時のモーション。
	case EnEnemyAnimation::enemyAnimation_death:
		m_ChaseEnemyRender.PlayAnimation(enemyAnimation_death, 0.1f);
		break;
	default:
		break;
	}
}

/// <summary>
/// アニメーションをセット。
/// </summary>
void ChaseEnemy::SetEnemyAnimation()
{
	// 待機モーション。
	FetchFullPath_EnemyAnimation(enemyAnimation_idle, "SkeletonIdle", true);

	// 歩きモーション。
	FetchFullPath_EnemyAnimation(enemyAnimation_walk, "SkeletonWalk", true);

	// 走りモーション。(追従用。)
	FetchFullPath_EnemyAnimation(enemyAnimation_run,  "SkeletonRun", true);

	// 死亡モーション。
	FetchFullPath_EnemyAnimation(enemyAnimation_death,"SkeletonDeath", false);

}