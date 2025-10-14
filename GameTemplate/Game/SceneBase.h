#pragma once

class Scene : public IGameObject
{
public:
	Scene() {};
	~Scene() {};
	bool Start() { return true; };
	void Update();
};

class SceneBase
{
public:
	SceneBase() {};
	virtual ~SceneBase() {};

	virtual bool Start() = 0;
	virtual void Update() = 0;
};

