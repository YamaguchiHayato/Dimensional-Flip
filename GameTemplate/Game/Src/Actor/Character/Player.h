#pragma once

enum PlayerState 
{
    sIdle,
	sRun,
	sJump,
	sNum,
};

enum EnAnimationClip
{
	Idle,
	Jump,
	Run,
	Num,
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
    void Action();

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
    // 座標。
    inline void SetPlayerPos(const Vector3& pos)
    {
        // 座標設定。
        pos_ = pos;
        // キャラコンを移動。
        charaCon_.SetPosition(pos_);
        // モデルの移動。
        render_.SetPosition(pos_);
    }
    // 一時停止フラグ。
    inline void SetPaused(bool isPaused)
    {
        isPaused_ = isPaused;
    }
    // セッターここまで。

// ゲッター。
public:
    // ジャンプ力の取得。
    inline const float& GetJumpPower() const { return jumpPower_; }
    // プレイヤーのCC取得。
	inline CharacterController& GetPlayerCC()
	{
		return charaCon_;
	}
    // プレイヤーの座標の取得。
	inline const Vector3 GetPlayerPos() const
	{ 
		return pos_; 
	}
    // プレイヤーの前方向ベクトル取得。
	inline const Vector3 GetForward() const
	{
		return forward_;

    }
    // トリガーエリア内かどうかの取得。
	inline bool GetInTriggerArea() const
	{
		return triggerOverlapCount_ > 0;
	}
    // カメラモードを取得。
    inline CameraManager* GetCameraManager() 
    {
        return pCameraManager_;
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
	AnimationClip animationClip_[EnAnimationClip::Num];
	CharacterController charaCon_;               

	ModelRender render_;
	Vector3 diff_;
	Vector3 moveSpeed_ = Vector3::Zero;
	Vector3 pos_ = Vector3::Zero;
    Vector3 forward_ = Vector3::Front;
	Quaternion rot_;

private:	
	int	state_; 
    //トリガーエリア内フラグ
	int triggerOverlapCount_ = 0; /// いくつのエリアに重なっているかカウント。

	float jumpPower_ = 50.0f;

	bool didJumpThisFrame_ = false;
    bool isPaused_ = false;  
};
