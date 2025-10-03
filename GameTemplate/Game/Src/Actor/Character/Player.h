#pragma once

enum PlayerState {
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
	void SetJumpPower(float jumpPower) { m_jumpPower = jumpPower; }

	/*
	 * @brief ジャンプ力の取得。
	 */
	const float& GetJumpPower() const { return m_jumpPower; }

	/*
	 * リスポーン地点の設定。 
	 */
	void SetRespawnPositon(const Vector3& pos)
	{
		m_respawnPos = pos;
	}

	/*
	 * プレイヤーをリスポーンする。 
	 */
	void PlayerRespawn()
	{
		m_position = m_respawnPos;
		m_rotation = m_respawnRot;
		m_modelRender.SetPosition(m_position);
		m_modelRender.SetRotation(m_rotation);
		m_Characon.SetPosition(m_position);
		m_respawnFlag = true;
	}


	/*
	 * プレイヤーがリスポーンしたか？ 
	 * @return リスポーンしたらtrue、
	 * してなかったらfalse
	 */
	bool IsPlayerRespawn()
	{
		return m_respawnFlag;
	}

	CharacterController& GetCharacterController()
	{
		return m_Characon;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	void AddMoveSpeed(const Vector3& addMoveSpeed)
	{
		m_moveSpeed += addMoveSpeed;
	}

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
	Player* m_player = nullptr;                                
	GameCamera* m_gameCamera = nullptr;
public:
	AnimationClip m_animationClip[enAnimationClip_Num];
	CharacterController m_Characon;               

	ModelRender m_modelRender;
	Vector3 diff;
	Vector3 m_moveSpeed;
	Vector3 m_position;
	Vector3 m_respawnPos;
	Quaternion m_respawnRot;
	Quaternion m_rotation;

private:	
	int	m_playerState; 
	int m_doCheck = 0;

	float m_jumpPower = 50.0f;
	bool m_respawnFlag = false;
	bool m_is2D = false;
};