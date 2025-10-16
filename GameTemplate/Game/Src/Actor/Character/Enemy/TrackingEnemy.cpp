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
	animationclip_[enemyAnimation].Load(Animation.c_str());
	animationclip_[enemyAnimation].SetLoopFlag(flag);

	return Animation;
}

// 初期化処理。
bool TrackingEnemy::Start()
{

	// アニメーションの設定。
	SetEnemyAnimation();

	// モデルの初期化。
	enemyRender_.Init("Assets/modelData/Skeleton.tkm", animationclip_, enAnimationclip_num, enModelUpAxisY, true);

	// 大きさをセット。
	enemyRender_.SetScale(SCALE);

	// キャラコン。
	enemyCC_.Init(RADIUS, HEIGHT, enemyPosition_);

	// 座標をセット。
	enemyRender_.SetPosition(enemyPosition_);

	// 更新処理。
	enemyRender_.Update();

	// エフェクトの初期化。
//	EffectEngine::GetInstance()->ResistEffect(EffectList_EnemyHit, u"Assets/effect/enemyhiteffect.efk");

	// 探索処理。
	player_ = FindGO<Player>("player");


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
	enemyRender_.SetPosition(enemyPosition_);

	// 更新処理。
	enemyRender_.Update();

	Vector3 diff = player_->PlayerPos_ - enemyPosition_;
	if (diff.Length() <= 100.0f && enemyanimationstate_ != 1)
	{
		if (player_->PlayerCharacon_.IsOnGround() == false)
		{
			enemyanimationstate_ = 1;
			player_->moveSpeed_.y = 500.0f;
			enemyCC_.RemoveRigidBoby();

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
		    touchPlayerFlag_ = true;
		}
	}

}

// 動作処理。
void TrackingEnemy::Move()
{
	if (!isChasing_)
	{
		if (enemystate_ == 0) {
			enemyMS_.x = -2.0f;
		}
		else if (enemystate_ == 1) {
			enemyMS_.x = 2.0f;
		}
		if (enemyPosition_.x >= enemyFP_.x + 200.0f)
		{
			enemystate_ = 0;
		}
		else if (enemyPosition_.x <= enemyFP_.x - 200.0f)
		{
			enemystate_ = 1;
		}

	}

	if (enemyanimationstate_ == 1) {
		enemyMS_.x = 0;
		if (!enemyRender_.IsPlayingAnimation())
		{
			DeleteGO(this);
		}
	}
	enemyPosition_ = enemyCC_.Execute(enemyMS_, 1.0f);
	float glavity = 3.0f;
	if (enemyCC_.IsOnGround()) {
		//重力をなくす
		enemyMS_.y = 0.0f;
	}
	enemyMS_.y -= glavity;
}

//追跡処理。
void TrackingEnemy::Tracking()
{
	Vector3 diff = player_->PlayerPos_ - enemyPosition_;
	const bool inRadius = (diff.Length() < 100.0f);

	// B) 座標トリガ（敵があるX座標を越えたら）
	const bool passX = (enemyPosition_.x >= triggerX_);

	if (!isChasing_ && (inRadius || passX)) {
		isChasing_ = true;
	}

	// ---- 追跡中の移動ベクトルを設定 ----
	if (isChasing_) {
		diff.y = 0.0f;                // 水平面だけで追う（必要ならYも可）
		if (diff.LengthSq() > 1e-4f) {
			diff.Normalize();
			enemyMS_.x = diff.x * chaseSpeed_;
			enemyMS_.z = diff.z * chaseSpeed_;
		}
		else {
			enemyMS_.x = enemyMS_.z = 0.0f; // ほぼ重なったら停止
		}
	}
}

// 回転処理。
void TrackingEnemy::Rotation() {
	if (enemystate_ == 0) {
		rotation_.SetRotationDegY(ENEMYSTATE_ZERO);
	}
	else if (enemystate_ == 1) {
		rotation_.SetRotationDegY(ENEMYSTATE_ONE);
	}
	rotation_.AddRotationDegX(ENEMYSTATE_TWO);
	//絵描きさんに回転を教える。
	enemyRender_.SetRotation(rotation_);
}

// Enemyのアニメーション。
void TrackingEnemy::EnemyAnimation()
{
	switch (enemyanimationstate_)
	{
	case 0:
		enemyRender_.PlayAnimation(enAnimationclip_walk, 0.1f);
		break;

	case 1:
		enemyRender_.PlayAnimation(enAnimationclip_death, 0.1f);
		break;
	}
}

// 描画処理。
void TrackingEnemy::Render(RenderContext& rc)
{
	enemyRender_.Draw(rc);
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