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

class CameraManager;
class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rendercontext)override;
    bool DoJumpCheck() const { return didJumpThisFrame_; };

private:
	void Move();
	void Rotation();
	void PlayAnimation();
	void ManageState();
    void AddMoveSpeed(const Vector3& addMoveSpeed)
	{
		moveSpeed_ += addMoveSpeed;
	}

// セッター。
public:
    // CameraManagerの初期化。
    inline void InitCameraManager(CameraManager* pCameraManager)
    {
        pCameraManager_ = pCameraManager;
    }
    // ジャンプ力の設定。
	inline void SetJumpPower(float jumpPower) { jumpPower_ = jumpPower; }
    // トリガーエリア内で呼ばれる処理。
    inline void EnterTriggerArea()
    {
        triggerOverlapCount_++;
    }
    // トリガーエリア外で呼ばれる処理。
	inline void ExitTriggerArea()
	{
        // 減算処理。
		triggerOverlapCount_--;
        // 0未満にならないように補正。
		if(triggerOverlapCount_ < 0)
		{
			triggerOverlapCount_ = 0;
		}
	}
// セッターここまで。

// ゲッター。
public:
    // ジャンプ力の取得。
    inline const float& GetJumpPower() const { return jumpPower_; }
    // プレイヤーのCC取得。
	inline CharacterController& GetPlayerCC()
	{
		return playerCC_;
	}
    // プレイヤーの座標の取得。
	inline const Vector3 GetPlayerPos() const
	{ 
		return playerPos_; 
	}
    // プレイヤーの前方向ベクトル取得。
	inline const Vector3 GetForward() const
	{
		return playerForward_;

    }
    // 戻るフラグの取得。
	inline bool GetReturnFlag() const
	{
		return returnFlag_;
	}
    // トリガーエリア内かどうかの取得。
	inline bool GetInTriggerArea() const
	{
		return triggerOverlapCount_ > 0;
	}
// ゲッターここまで。

private:
    // アニメーションを取得して再生する関数。
	const std::string FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName, bool flag);

    // モデルを取得して再生する関数。
	const std::string FetchPlayerModel
	(const std::string& modelName, AnimationClip animationClip, EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag);

    // アニメーションを格納。
	void SetAnimation();


private:
	Player* player_ = nullptr;
    CameraManager* pCameraManager_ = nullptr;
public:
	AnimationClip animationClip_[enAnimationClip_Num];
    // CC … CharacterController。
	CharacterController playerCC_;               

	ModelRender playerRender_;
	Vector3 diff_;
	Vector3 moveSpeed_ = Vector3::Zero;
	Vector3 playerPos_ = Vector3::Zero;
    Vector3 playerForward_ = Vector3::Front;
	Quaternion playerot_;

private:	
	int	playerState_; 
	int doCheck_ = 0;
    //トリガーエリア内フラグ
	int triggerOverlapCount_ = 0; /// いくつのエリアに重なっているかカウント。

	float jumpPower_ = 50.0f;

	bool didJumpThisFrame_ = false;
	bool returnFlag_ = false; //戻るフラグ

};
