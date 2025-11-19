#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"

enum class State
{
    Patrol,
	Chase
};

enum EnEnemyAnimation
{
	enIdle,
	enWalk,
	enDeath,
	enNum
};


class Player;
/// <summary>
/// 派生クラス : 追跡敵。
/// </summary>
class TrackingEnemy :  public IEnemy
// 外部参照用 関数。
{
public:

	TrackingEnemy() {};
	~TrackingEnemy() {};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
    inline const std::string InitModel(const std::string& gimmickname) override
    {
        return IEnemy::InitModel(gimmickname);
    };

public:
    inline const void SetPos(const Vector3& pos)
    {
        pos_ = pos;
    };

private:
    Player* pPlayer_ = nullptr;

	AnimationClip animationclip_[Num];
	CharacterController charaCon_;
	SphereCollider sphereColl_;
	Vector3 moveSpeed_;
    Vector3 initPos_;
	Vector3 pos_;
	Vector3 distanceX_ = Vector3::Zero;//x軸の判定
	Vector3 distanceY_ = Vector3::Zero;//y軸の判定
	ModelRender render_;
	Quaternion rot_;

private:
	uint8_t animaState_ = 0;

	bool  touchPlayerFlag_ = false;//プレイヤーに触れたか？
    bool  isChasing_ = false;   // 追跡中フラグ
	float chaseSpeed_ = 4.0f;   // 追跡速度
	float triggerX_ = FLT_MAX;



private:
    // 追跡処理。
	void Tracking();
    // 動作処理。
    void Move();
    // 回転処理。
	void Rotation();
    // Enemyのアニメーション。
	void EnemyAnimation();

    // アニメーションファイルパスを取得する関数。
	const std::string FetchAnimation(EnEnemyAnimation enemyAnimation, const std::string& animationName, bool flag);

    // Enemyのアニメーションをセットする関数。
	void SetEnemyAnimation();

private:

	const char* ENEMY_ANIMATION = "Assets/animData/";// ファイルパス。
	const char* ANIMATION_FILE_EXTENSION = ".tka"; // 拡張子。

};







