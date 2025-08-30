#pragma once
#include "stdafx.h"

class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};

	//初期化処理
	bool Start();

	//更新処理
	void Update();

	//描画処理
	void Render(RenderContext& rendercontext);

	//移動処理
	void Move();

	//回転処理
	void Rotation();

	// アニメーションの再生。
	void PlayAnimation();
	
	// プレイヤーステート。
	enum PlayerState {
		enPlayer_idle,
		enPlayer_walk,
		enPlayer_run,
		enPlayer_jump,
		enPlayer_stageclear,
		enPlayer_gameover,
		enPlayer_num
	};

	//アニメーション
	enum EnAnimationClip
	{
		enAnimationClip_Idle,                         //待機アニメ
		enAnimationClip_Walk,                         //歩行アニメ
		enAnimationClip_Jump,                         //ジャンプアニメ
		enAnimationClip_Run,                          //走行アニメ
		enAnimationClip_Num,                          //アニメーションの数
	};
	//メンバ変数
	Player* m_player;                                 //プレイヤー
	Vector3 m_position;                               //座標ベクトル
	ModelRender m_modelRender;                          //モデル描画クラス
	Quaternion rot;
	AnimationClip animationClip[enAnimationClip_Num]; //アニメーション
	CharacterController m_Characon;                   //キャラクターコントローラー
	Vector3 diff;
	Vector3 m_moveSpeed;                           //移動速度
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

	int	m_playerState = enPlayer_idle;


public:
	enum class Axis
	{
		XY,
		XYZ
	};

	void SetMovementAxis(Axis axis);

};

