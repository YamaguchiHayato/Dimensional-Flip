#pragma once
#include "Src/camera/FollowStrategy.h"
#include "Src/camera/ICameraStrategy.h"
#include "Src/Actor/Character/Player.h"
#include <DirectXMath.h>

class Player;
class SideCameraStrategy : public ICameraStrategy
{
public:

    // 暗黙的な型変換を禁止する。
	explicit SideCameraStrategy(Player* pPlayer);

	bool Start()override { return true; };
	void Update(nsK2EngineLow::Camera* pCamera, const float deltaTime) override;

public:
	inline const nsK2EngineLow::Quaternion& GetCurrentRotation() const
	{
		return currentRotation_;
	}
	

private:
	Player* pPlayer_ = nullptr;

private:
	Vector3 idealOffset_{0.0f, 10.0f, 0.0f};



private:
	float rotationSpeed_ = 5.0f; 

};
