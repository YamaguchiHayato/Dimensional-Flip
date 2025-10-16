#pragma once

enum PlayerState 
{
	enPlayer_idle,
	enPlayer_walk,
	enPlayer_run,
	enPlayer_jump,
	enPlayer_stageclear,
	enPlayer_gameover,
	enPlayer_num,
};

enum EnAnimationClip
{
	enAnimationClip_Idle,
	enAnimationClip_Walk,
	enAnimationClip_Jump,
	enAnimationClip_Run,
	enAnimationClip_Num,
};

class GameCamera;
class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rendercontext)override;
	void Move();
	void Rotation();
	void PlayAnimation();
	void ManageState();

	/*
	 * @brief ジャンプ力の設定。
	 */
	void SetJumpPower(float jumpPower) { jumpPower_ = jumpPower; }

	/*
	 * @brief ジャンプ力の取得。
	 */
	const float& GetJumpPower() const { return jumpPower_; }

	/*
	 * リスポーン地点の設定。 
	 */
	void SetRespawnPositon(const Vector3& pos)
	{
		respawnPos_ = pos;
	}

	/*
	 * プレイヤーをリスポーンする。 
	 */
	void PlayerRespawn()
	{
		PlayerPos_ = respawnPos_;
		rotation_ = respawnRot_;
		PlayerRender_.SetPosition(PlayerPos_);
		PlayerRender_.SetRotation(rotation_);
		PlayerCharacon_.SetPosition(PlayerPos_);
		respawnFlag_ = true;
	}


	/*
	 * プレイヤーがリスポーンしたか？ 
	 * @return リスポーンしたらtrue、
	 * してなかったらfalse
	 */
	bool IsPlayerRespawn()
	{
		return respawnFlag_;
	}

	CharacterController& GetCharacterController()
	{
		return PlayerCharacon_;
	}
	const Vector3& GetPosition() const
	{
		return PlayerPos_;
	}
	void AddMoveSpeed(const Vector3& addMoveSpeed)
	{
		moveSpeed_ += addMoveSpeed;
	}

	bool DoJumpCheck() const { return didJumpThisFrame_; };
private:

	/**
	  * @brief アニメーションを取得して再生する関数。
	  */
	const std::string FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName, bool flag);

	/**
	 * @brief プレイヤーモデルを取得して再生する関数。
	 * @param modelName 
	 * @param animationClip 
	 * @param enAnimationClip 
	 * @param enModelUpAxis 
	 * @param flag 
	 * @return 
	 */
	const std::string FetchPlayerModel
	(const std::string& modelName, AnimationClip animationClip, EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag);

	/**
	  * @brief アニメーションを格納する関数。
	  */
	void SetAnimation();


private:
	Player* player_ = nullptr;                                
	GameCamera* gameCamera_ = nullptr;
public:
	AnimationClip animationClip_[enAnimationClip_Num];
	CharacterController PlayerCharacon_;               

	ModelRender PlayerRender_;
	Vector3 diff_;
	Vector3 moveSpeed_;
	Vector3 PlayerPos_ = Vector3::Zero;
	Vector3 respawnPos_;
	Quaternion respawnRot_;
	Quaternion rotation_;

private:	
	int	playerState_; 
	int doCheck_ = 0;

	float jumpPower_ = 50.0f;
	bool respawnFlag_ = false;
	bool is2D_ = false;
	bool didJumpThisFrame_ = false;
};