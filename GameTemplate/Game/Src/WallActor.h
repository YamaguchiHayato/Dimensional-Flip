#pragma once
#include "Src/Actor/Actor.h"
class WallActor : public Actor
{
public:
	WallActor() {};
	~WallActor();

	bool Start() override;
	void Update() override;
    void Render(RenderContext& rc) override {};

public:
    inline void SetWallRot(const Quaternion& rot) { m_WallRot = rot; }
	inline void SetWallPos(const Vector3& pos) { m_WallPos = pos; }

public:
	inline const Vector3& GetWallPos() const  { return m_WallPos; }

private:
	ModelRender m_WallRender;
	Vector3 m_WallPos = Vector3::Zero;
	CollisionObject* m_Collision = nullptr;
	Quaternion m_WallRot = Quaternion::Identity;
	PhysicsStaticObject m_physicsStaticObject;
};

