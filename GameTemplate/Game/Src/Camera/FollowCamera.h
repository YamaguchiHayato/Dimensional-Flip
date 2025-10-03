#pragma once
#include "ICameraStrategy.h"
class Player;
class FollowCamera : public IGameObject
{
public:
	FollowCamera() {};
	~FollowCamera() {};

	bool Start();
	void Update();
	void Move();

private:
	Player* m_player = nullptr;

private:
	Quaternion qRot;
	Vector3 axisX;
	Vector3 m_CameraPos = Vector3::Zero;
//	void Update(GameCamera* owner) override;
};

