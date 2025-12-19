#pragma once
#include "Src/camera/ICameraStrategy.h"
#include "Src/Actor/Character/Player.h"
#include <DirectXMath.h>

class Player;
class FollowStrategy : public ICameraStrategy
{
public:
    // 暗黙的な型変換を禁止する。
	explicit FollowStrategy(Player* pPlayer);
	virtual ~FollowStrategy() {};

	bool Start() override;
	void Update(nsK2EngineLow::Camera* camera, const float deltaTime) override;

    static Vector3 Lerp(float t, const Vector3& v0, const Vector3& v1)
	{
		DirectX::XMVECTOR resultVec = DirectX::XMVectorLerp(
			DirectX::XMLoadFloat3(&v0.vec),
			DirectX::XMLoadFloat3(&v1.vec),
			t
		);
		
		Vector3 result;
		DirectX::XMStoreFloat3(&result.vec, resultVec);
		return result;
	}
private:
	Player* pPlayer_ = nullptr;

private:	
	Quaternion playerRot_;
	Vector3 axisX;
};
