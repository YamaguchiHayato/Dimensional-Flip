#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	// キャラコンの定数。
	const float RADIUS = 25.0f;
	const float HEIGHT = 100.0f;

	// EnemyStateの定数。
	const float ENEMYSTATE_ZERO = 270.0f;
	const float ENEMYSTATE_ONE = 90.0f;
	const float ENEMYSTATE_TWO = -180.0f;

	// スケール。
	const Vector3 SCALE(5.0f, 5.0f, 5.0f);
}


// アニメーションメソッド。
const::std::string TrackingEnemy::GetFullPath_EnemyAnimation(EnAnimationClip enemyAnimation, const std::string& animationName, bool flag)
{
	std::string Animation = ENEMY_ANIMATION + animationName + ANIMATION_FILE_EXTENSION;

	// アニメーションのロード。
	m_animationclip[enemyAnimation].Load(Animation.c_str());
	m_animationclip[enemyAnimation].SetLoopFlag(flag);

	return Animation;
}

// 初期化処理。
bool TrackingEnemy::Start()
{

	// アニメーションの設定。
	SetEnemyAnimation();

	// モデルの初期化。
	m_EnemyRender.Init("Assets/modelData/Skeleton.tkm", m_animationclip, enAnimationclip_num, enModelUpAxisY, true);

	// 大きさをセット。
	m_EnemyRender.SetScale(SCALE);

	// キャラコン。
	m_EnemyCC.Init(RADIUS, HEIGHT, m_EnemyPosition);

	// 座標をセット。
	m_EnemyRender.SetPosition(m_EnemyPosition);

	// 更新処理。
	m_EnemyRender.Update();

	// エフェクトの初期化。
//	EffectEngine::GetInstance()->ResistEffect(EffectList_EnemyHit, u"Assets/effect/enemyhiteffect.efk");

	// 探索処理。
	m_player = FindGO<Player>("player");


	return true;
}

// 更新処理。
void TrackingEnemy::Update()
{
	// 追跡処理。
	Tracking();

	// 動作処理。
	Move();

	// 回転処理。
	Rotation();

	// アニメーション。
	EnemyAnimation();

	// モデルをセット。
	m_EnemyRender.SetPosition(m_EnemyPosition);

	// 更新処理。
	m_EnemyRender.Update();

	Vector3 diff = m_player->PlayerPos_ - m_EnemyPosition;
	if (diff.Length() <= 100.0f && Enemyanimationstate != 1)
	{
		if (m_player->PlayerCharacon_.IsOnGround() == false)
		{
			Enemyanimationstate = 1;
			m_player->moveSpeed_.y = 500.0f;
			m_EnemyCC.RemoveRigidBoby();

			////エフェクトの処理
			//EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
			//effectEmitter->Init(EffectList_EnemyHit);

			////エフェクトの位置の設定
			//Vector3 enemyLocalPos = { 0.0f,120.0f,0.0f };
			//enemyLocalPos += m_position;
			//effectEmitter->SetPosition(enemyLocalPos);

			////エフェクトの大きさの設定
			//effectEmitter->SetScale({ 30.0f,30.0f,30.0f });

			////エフェクトの再生
			//effectEmitter->Play();

			////プレイヤーが敵を踏んだ時の音を再生。
			//g_gameSoundEngine->PlaySE(GameSoundList_SE_Player_StepOnEnemy, 1.0f);
		}
		else
		{
			//プレイヤーに触れた
			m_touchPlayerFlag = true;
		}
	}

}

// 動作処理。
void TrackingEnemy::Move()
{
	if (!m_isChasing)
	{
		if (Enemystate == 0) {
			m_EnemyMS.x = -2.0f;
		}
		else if (Enemystate == 1) {
			m_EnemyMS.x = 2.0f;
		}
		if (m_EnemyPosition.x >= m_EnemyFP.x + 200.0f)
		{
			Enemystate = 0;
		}
		else if (m_EnemyPosition.x <= m_EnemyFP.x - 200.0f)
		{
			Enemystate = 1;
		}

	}

	if (Enemyanimationstate == 1) {
		m_EnemyMS.x = 0;
		if (!m_EnemyRender.IsPlayingAnimation())
		{
			DeleteGO(this);
		}
	}
	m_EnemyPosition = m_EnemyCC.Execute(m_EnemyMS, 1.0f);
	float glavity = 3.0f;
	if (m_EnemyCC.IsOnGround()) {
		//重力をなくす
		m_EnemyMS.y = 0.0f;
	}
	m_EnemyMS.y -= glavity;
}

//追跡処理。
void TrackingEnemy::Tracking()
{
	Vector3 diff = m_player->PlayerPos_ - m_EnemyPosition;
	const bool inRadius = (diff.Length() < 100.0f);

	// B) 座標トリガ（敵があるX座標を越えたら）
	const bool passX = (m_EnemyPosition.x >= m_triggerX);

	if (!m_isChasing && (inRadius || passX)) {
		m_isChasing = true;
	}

	// ---- 追跡中の移動ベクトルを設定 ----
	if (m_isChasing) {
		diff.y = 0.0f;                // 水平面だけで追う（必要ならYも可）
		if (diff.LengthSq() > 1e-4f) {
			diff.Normalize();
			m_EnemyMS.x = diff.x * m_chaseSpeed;
			m_EnemyMS.z = diff.z * m_chaseSpeed;
		}
		else {
			m_EnemyMS.x = m_EnemyMS.z = 0.0f; // ほぼ重なったら停止
		}
	}
}

// 回転処理。
void TrackingEnemy::Rotation() {
	if (Enemystate == 0) {
		rotation.SetRotationDegY(ENEMYSTATE_ZERO);
	}
	else if (Enemystate == 1) {
		rotation.SetRotationDegY(ENEMYSTATE_ONE);
	}
	rotation.AddRotationDegX(ENEMYSTATE_TWO);
	//絵描きさんに回転を教える。
	m_EnemyRender.SetRotation(rotation);
}

// Enemyのアニメーション。
void TrackingEnemy::EnemyAnimation()
{
	switch (Enemyanimationstate)
	{
	case 0:
		m_EnemyRender.PlayAnimation(enAnimationclip_walk, 0.1f);
		break;

	case 1:
		m_EnemyRender.PlayAnimation(enAnimationclip_death, 0.1f);
		break;
	}
}

// 描画処理。
void TrackingEnemy::Render(RenderContext& rc)
{
	m_EnemyRender.Draw(rc);
}

// アニメーションの再生。
void TrackingEnemy::SetEnemyAnimation()
{
	// 待機モーション。
	GetFullPath_EnemyAnimation(enAnimationclip_idle, "SkeletonIdle", true);

	// 歩きモーション。
	GetFullPath_EnemyAnimation(enAnimationclip_walk, "SkeletonWalk", true);

	// 死亡モーション。
	GetFullPath_EnemyAnimation(enAnimationclip_death, "SkeletonDeath", false);

}