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
	Vector3 m_position;
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
	Quaternion rot;

private:	
	int	m_playerState = enPlayer_idle;

	bool m_is2D = false;
};