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
	ModelRender modelRender;	
	Vector3 m_position;

	PhysicsStaticObject m_physicsStaticObject;
	CollisionObject* m_collisionObject = nullptr;

};

