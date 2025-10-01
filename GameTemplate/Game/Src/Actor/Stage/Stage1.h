#pragma once
#include "Src/Actor/Stage/Stage.h"



class Stage1 : public IStage
{
public:
	Stage1() {};
	~Stage1();
	bool Start()override;
	void Update()override;
	void Render(RenderContext & rc)override;

private:
	ModelRender m_StageRender;	

	Vector3 m_position;
	Vector3 m_initPosition;

	PhysicsStaticObject m_physicsStaticObject;

	Quaternion m_StageRot;
private:
	CollisionObject* m_collisionObject = nullptr;

};

