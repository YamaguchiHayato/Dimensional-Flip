#pragma once




class Stage1 : public IGameObject
{
public:
	Stage1() {};
	~Stage1();
	bool Start();
	void Update();
	void Render(RenderContext & rc);

private:
	ModelRender m_StageRender;	

	Vector3 m_position;
	Vector3 m_initPosition;

	PhysicsStaticObject m_physicsStaticObject;

	Quaternion m_StageRot;
private:
	CollisionObject* m_collisionObject = nullptr;

};

